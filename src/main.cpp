#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <clang/Lex/Lexer.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <TextOperationApplier.hpp>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

TextOperationApplier sourceOperations;
std::string extractedMethodName;

struct RowColumn
{
    unsigned row, column;
    RowColumn() : row(0), column(0) { }
    RowColumn(unsigned row, unsigned column) : row(row), column(column) { }
};

struct UserSourceSelection
{
    RowColumn from, to;
};

UserSourceSelection extractMethodSelection;

// TODO: semicolon hack

template <typename Node>
unsigned extraCharsHack(const Node&) { return 0; }

template <>
unsigned extraCharsHack<Stmt>(const Stmt&) { return 1; } // semicolon

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }
    
    template <typename Node>
    clang::SourceRange getCorrectSourceRange(const Node& node)
    {
        auto spelling = getSpellingRange(node);
        auto sourceLength = getSourceLength(spelling, node);
        return SourceRange(spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength));
    }

    std::string getSource(SourceRange range)
    {
        return std::string(getText(range.getBegin()), rangeLength(range));
    }

private:
    clang::SourceManager& sourceManager;
    
    template <typename Node>
    clang::SourceRange getSpellingRange(const Node& n)
    {
        auto r = SourceRange(
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

    unsigned locOffset(SourceLocation loc)
    {
        return sourceManager.getFileOffset(loc);
    }
    
    unsigned locDistance(SourceLocation from, SourceLocation to)
    {
        return locOffset(to) - locOffset(from);
    }
    
    unsigned rangeLength(SourceRange r)
    {
        return locDistance(r.getBegin(), r.getEnd());
    }
    
    const char *getText(SourceLocation loc)
    {
        auto invalid = true;
        auto text = sourceManager.getCharacterData(loc, &invalid);
        if (invalid)
            throw std::runtime_error("cannot get characted data");
        return text;
    }
};

class MethodExtractor : public RecursiveASTVisitor<MethodExtractor>
{
    ASTContext& ctx;
    SourceExtractor sourceExtractor{ctx.getSourceManager()};
    std::string extractedMethodName;
    UserSourceSelection selection;
public:
    MethodExtractor(ASTContext& ctx, const std::string& extractedMethodName, UserSourceSelection selection)
        : ctx(ctx), extractedMethodName(extractedMethodName), selection(selection) { }
    bool VisitFunctionDecl(FunctionDecl* decl)
    {
        if (!ctx.getSourceManager().isFromMainFile(decl->getLocation()) || !decl->hasBody())
            return true;
        auto stmts = findStatements(*decl, selection);
        printExtractedFunction(sourceExtractor.getCorrectSourceRange(*decl).getBegin(), extractedMethodName, stmts);
        printOriginalFunctionWithExtractedFunctionCall(extractedMethodName, *decl, stmts);
        return false;
    }
private:
    bool overlaps(clang::SourceRange r, UserSourceSelection s)
    {
        auto begin = ctx.getSourceManager().getPresumedLoc(r.getBegin());
        auto end = ctx.getSourceManager().getPresumedLoc(r.getEnd());
        if (s.to.row < begin.getLine()) return false;
        if (s.from.row > end.getLine()) return false;
        if (s.from.row < end.getLine()) return true;
        if (s.to.row > begin.getLine()) return true;
        
        return true;
    }
    bool overlaps(clang::Stmt *stmt, UserSourceSelection s)
    {
        return overlaps(sourceExtractor.getCorrectSourceRange(*stmt), s);
    }
    clang::SourceRange getSourceRange(Stmt::const_child_range stmts)
    {
        clang::SourceRange r;
        r.setBegin(sourceExtractor.getCorrectSourceRange(**stmts).getBegin());
        for (auto s : stmts)
            r.setEnd(sourceExtractor.getCorrectSourceRange(*s).getEnd());
        return r;
    }
    clang::Stmt::const_child_range findStatements(const FunctionDecl& func, UserSourceSelection selection)
    {
        auto body = func.getBody();
        auto begin =
            std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return overlaps(s, selection); });
        auto end =
            std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !overlaps(s, selection); });
        return {begin, end};
    }
    void printExtractedFunction(SourceLocation at, const std::string& name, Stmt::const_child_range stmts)
    {
        auto& sm = ctx.getSourceManager();
        std::ostringstream os;
        os << "void " << name << "()\n{\n    " << sourceExtractor.getSource(getSourceRange(stmts)) << "\n}\n";
        sourceOperations.insertTextAt(os.str(), sm.getFileOffset(at));
    }
    void printOriginalFunctionWithExtractedFunctionCall(const std::string& name, FunctionDecl& decl, Stmt::const_child_range stmts)
    {
        getTextWithReplace(getSourceRange(stmts), name + "();");
    }
    void getTextWithReplace(SourceRange without, std::string replace)
    {
        auto& sm = ctx.getSourceManager();
        sourceOperations.removeTextInRange(sm.getFileOffset(without.getBegin()), sm.getFileOffset(without.getEnd()));
        sourceOperations.insertTextAt(replace, sm.getFileOffset(without.getBegin()));
    }
};

class MethodExtractorUnitHandler : public ASTConsumer
{
public:
    MethodExtractorUnitHandler() { }
    virtual void HandleTranslationUnit(ASTContext& ctx)
    {
        MethodExtractor extractor(ctx, extractedMethodName, extractMethodSelection);
        extractor.TraverseDecl(ctx.getTranslationUnitDecl());
    }
};

class MethodExtractorFactory: public ASTFrontendAction
{
public:
    virtual ASTConsumer* CreateASTConsumer(CompilerInstance&, StringRef)
    {
        return new MethodExtractorUnitHandler();
    }
};

unsigned to_u(const char *s)
{
    std::istringstream ss(s);
    unsigned u;
    ss >> u;
    return u;
}

std::string loadFile(const std::string& filename)
{
    std::ifstream f(filename);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

int main(int argc, const char** argv)
{
    int fakeArgc = 3;
    const char *fakeArgv[] = { argv[0], argv[1], "--" };
    CommonOptionsParser parser(fakeArgc, fakeArgv);
    ClangTool tool(parser.GetCompilations(), parser.GetSourcePathList());
 
    extractedMethodName = argv[3];
    extractMethodSelection.from = RowColumn{ to_u(argv[4]), to_u(argv[5]) };
    extractMethodSelection.to = RowColumn{ to_u(argv[6]), to_u(argv[7]) };
    
    tool.run(newFrontendActionFactory<MethodExtractorFactory>());
    
    std::string source = loadFile(argv[1]);
    std::string modifiedSource = sourceOperations.apply(source);
    std::ofstream f(argv[1]);
    f << modifiedSource;
}