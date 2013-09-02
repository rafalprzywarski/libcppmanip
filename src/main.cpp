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
#include "SourceExtractor.hpp"
#include "ClangToolArgsBuilder.hpp"

class MethodExtractor : public clang::RecursiveASTVisitor<MethodExtractor>
{
    clang::ASTContext& ctx;
    SourceExtractor sourceExtractor{ctx.getSourceManager()};
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
public:
    MethodExtractor(clang::ASTContext& ctx, const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : ctx(ctx), extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
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
    MethodExtractorUnitHandler(const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx)
    {
        MethodExtractor extractor(ctx, extractedMethodName, selection, sourceOperations);
        extractor.TraverseDecl(ctx.getTranslationUnitDecl());
    }
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

class MethodExtractorFactory: public clang::ASTFrontendAction
{
public:
    MethodExtractorFactory(const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance&, clang::StringRef)
    {
        return new MethodExtractorUnitHandler(extractedMethodName, selection, sourceOperations);
    }
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

class MethodExtractorFactoryFactory : public clang::tooling::FrontendActionFactory
{
public:
    MethodExtractorFactoryFactory(const std::string& extractedMethodName, UserSelection selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection.from, selection.to), sourceOperations(sourceOperations) { }
    virtual clang::FrontendAction* create()
    {
        return new MethodExtractorFactory(extractedMethodName, selection, sourceOperations);
    }
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

void performFrontendActionClangToolForFile(clang::tooling::FrontendActionFactory& actionFactory, std::string sourceFilename)
{
    ClangToolArgsBuilder args;
    args.setSourceFilename(sourceFilename);
    auto argc = args.getArgc();
    auto argv = args.getArgv();
    clang::tooling::CommonOptionsParser parser(argc, argv);
    clang::tooling::ClangTool tool(parser.GetCompilations(), parser.GetSourcePathList());
    tool.run(&actionFactory);
}

void applySourceOperationsToFile(TextOperationApplier& sourceOperations, const std::string& filename)
{
    std::string source = loadTextFromFile(filename);
    std::string modifiedSource = sourceOperations.apply(source);
    writeTextToFile(modifiedSource, filename); 
}

void extractMethodInFile(const std::string& methodName, UserSelection selection, const std::string& filename)
{
    TextOperationApplier sourceOperations;
    MethodExtractorFactoryFactory extractorFactoryFactory(methodName, selection, sourceOperations);
    performFrontendActionClangToolForFile(extractorFactoryFactory, filename);
    applySourceOperationsToFile(sourceOperations, filename);
}

void performOperation(const OperationRequest& req)
{
    extractMethodInFile(req.extractedMethodName, req.sourceSelection, req.sourceFilename);
}

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto operationRequest = parser.parse(argc, argv);
        performOperation(operationRequest);
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