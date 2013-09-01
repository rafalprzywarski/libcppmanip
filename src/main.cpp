#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <clang/Lex/Lexer.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <TextOperationApplier.hpp>
#include "OffsetRange.hpp"
#include "CommandLineParser.hpp"
#include "TextFileOps.hpp"

TextOperationApplier sourceOperations;
std::string extractedMethodName;

OffsetRange extractMethodSelection;

// TODO: semicolon hack

template <typename Node>
unsigned extraCharsHack(const Node&) { return 0; }

template <>
unsigned extraCharsHack<clang::Stmt>(const clang::Stmt&) { return 1; } // semicolon

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }
    
    template <typename Node>
    clang::SourceRange getCorrectSourceRange(const Node& node)
    {
        auto spelling = getSpellingRange(node);
        auto sourceLength = getSourceLength(spelling, node);
        return {spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength)};
    }

    clang::SourceRange getCorrectSourceRange(clang::ConstStmtRange stmts)
    {
        clang::SourceRange r;
        r.setBegin(getCorrectSourceRange(**stmts).getBegin());
        for (auto s : stmts)
            r.setEnd(getCorrectSourceRange(*s).getEnd());
        return r;
    }

    std::string getSource(clang::ConstStmtRange stmts)
    {
        auto range = getCorrectSourceRange(stmts);
        return std::string(getText(range.getBegin()), rangeLength(range));
    }

private:
    clang::SourceManager& sourceManager;
    
    template <typename Node>
    clang::SourceRange getSpellingRange(const Node& n)
    {
        auto r = clang::SourceRange(
            sourceManager.getSpellingLoc(n.getSourceRange().getBegin()),
            sourceManager.getSpellingLoc(n.getSourceRange().getEnd()));
        if (r.isInvalid())
            throw std::runtime_error("cannot get spelling range");
        return r;
    }

    template <typename Node>
    unsigned getSourceLength(clang::SourceRange spelling, const Node& node)
    {
        auto start = locOffset(spelling.getBegin());
        auto end = sourceManager.getDecomposedLoc(
            clang::Lexer::getLocForEndOfToken(spelling.getEnd(), 0, sourceManager, clang::LangOptions())).second;
        if (end < start)
            throw std::runtime_error("invalid decomposed range, probably because of macros");
        return end - start + extraCharsHack(node);
    }

    unsigned locOffset(clang::SourceLocation loc)
    {
        return sourceManager.getFileOffset(loc);
    }
    
    unsigned locDistance(clang::SourceLocation from, clang::SourceLocation to)
    {
        return locOffset(to) - locOffset(from);
    }
    
    unsigned rangeLength(clang::SourceRange r)
    {
        return locDistance(r.getBegin(), r.getEnd());
    }
    
    const char *getText(clang::SourceLocation loc)
    {
        auto invalid = true;
        auto text = sourceManager.getCharacterData(loc, &invalid);
        if (invalid)
            throw std::runtime_error("cannot get characted data");
        return text;
    }
};

class MethodExtractor : public clang::RecursiveASTVisitor<MethodExtractor>
{
    clang::ASTContext& ctx;
    SourceExtractor sourceExtractor{ctx.getSourceManager()};
    std::string extractedMethodName;
    OffsetRange selection;
public:
    MethodExtractor(clang::ASTContext& ctx, const std::string& extractedMethodName, OffsetRange selection)
        : ctx(ctx), extractedMethodName(extractedMethodName), selection(selection) { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!ctx.getSourceManager().isFromMainFile(decl->getLocation()) || !decl->hasBody())
            return true;
        auto stmts = findStatements(*decl, selection);
        printExtractedFunction(sourceExtractor.getCorrectSourceRange(*decl).getBegin(), extractedMethodName, stmts);
        printOriginalFunctionWithExtractedFunctionCall(extractedMethodName, *decl, stmts);
        return false;
    }
private:
    bool overlaps(clang::SourceRange r, OffsetRange s)
    {
        auto begin = ctx.getSourceManager().getFileOffset(r.getBegin());
        auto end = ctx.getSourceManager().getFileOffset(r.getEnd());
        return s.overlapsWith({begin, end});
    }
    bool overlaps(clang::Stmt *stmt, OffsetRange s)
    {
        return overlaps(sourceExtractor.getCorrectSourceRange(*stmt), s);
    }
    clang::ConstStmtRange findStatements(const clang::FunctionDecl& func, OffsetRange selection)
    {
        auto body = func.getBody();
        auto begin =
            std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return overlaps(s, selection); });
        auto end =
            std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !overlaps(s, selection); });
        return {begin, end};
    }
    void printExtractedFunction(clang::SourceLocation at, const std::string& name, clang::ConstStmtRange stmts)
    {
        auto& sm = ctx.getSourceManager();
        std::ostringstream os;
        os << "void " << name << "()\n{\n    " << sourceExtractor.getSource(stmts) << "\n}\n";
        sourceOperations.insertTextAt(os.str(), sm.getFileOffset(at));
    }
    void printOriginalFunctionWithExtractedFunctionCall(const std::string& name, clang::FunctionDecl& decl, clang::ConstStmtRange stmts)
    {
        replaceRangeWith(sourceExtractor.getCorrectSourceRange(stmts), name + "();");
    }
    void replaceRangeWith(clang::SourceRange without, std::string replace)
    {
        auto& sm = ctx.getSourceManager();
        sourceOperations.removeTextInRange(sm.getFileOffset(without.getBegin()), sm.getFileOffset(without.getEnd()));
        sourceOperations.insertTextAt(replace, sm.getFileOffset(without.getBegin()));
    }
};

class MethodExtractorUnitHandler : public clang::ASTConsumer
{
public:
    MethodExtractorUnitHandler() { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx)
    {
        MethodExtractor extractor(ctx, extractedMethodName, extractMethodSelection);
        extractor.TraverseDecl(ctx.getTranslationUnitDecl());
    }
};

class MethodExtractorFactory: public clang::ASTFrontendAction
{
public:
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance&, clang::StringRef)
    {
        return new MethodExtractorUnitHandler();
    }
};

void runClangToolForFile(const std::string& filename)
{
    int fakeArgc = 3;
    const char *fakeArgv[] = { "", filename.c_str(), "--" };
    clang::tooling::CommonOptionsParser parser(fakeArgc, fakeArgv);
    clang::tooling::ClangTool tool(parser.GetCompilations(), parser.GetSourcePathList());
    tool.run(clang::tooling::newFrontendActionFactory<MethodExtractorFactory>());
}

void applySourceOperationsToFile(const std::string& filename)
{
    std::string source = loadTextFromFile(filename);
    std::string modifiedSource = sourceOperations.apply(source);
    writeTextToFile(modifiedSource, filename); 
}

void setMethodExtractorForNameAndSourceSelection(const std::string& methodName, OffsetRange sourceRange)
{
    extractedMethodName = methodName;
    extractMethodSelection = sourceRange;
}

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser args;
        args.parse(argc, argv);
        
        setMethodExtractorForNameAndSourceSelection(args.getExtractedMethodName(), args.getSourceSelection());
        runClangToolForFile(args.getSourceFilename());
        applySourceOperationsToFile(args.getSourceFilename());
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown internal error" << std::endl;
        return 1;
    }
}