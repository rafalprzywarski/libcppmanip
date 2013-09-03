#include "CppManip.hpp"
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
#include "TextOperationApplier.hpp"
#include "OffsetRange.hpp"
#include "TextFileOps.hpp"
#include "SourceExtractor.hpp"
#include "ClangToolArgsBuilder.hpp"
#include <functional>

class MethodExtractor : public clang::RecursiveASTVisitor<MethodExtractor>
{
    SourceExtractor& sourceExtractor;
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
public:
    MethodExtractor(SourceExtractor& sourceExtractor, const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : sourceExtractor(sourceExtractor), extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        handleFunctionDecl(*decl);
        return true;
    }
private:

    bool doesNotContainSelection(const clang::FunctionDecl& f)
    {
        return !sourceExtractor.isLocationFromMainFile(f.getLocation()) || !f.hasBody();
    }

    void handleFunctionDecl(const clang::FunctionDecl& decl)
    {
        if (doesNotContainSelection(decl))
            return;
        auto stmts = findStatementsTouchingSelection(decl);
        extractStatmentsFromFunction(stmts, decl);
    }

    void extractStatmentsFromFunction(clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction)
    {
        auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
        auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
        printExtractedFunction(originalFunctionLocation, extractedMethodName, stmtsRange);
        replaceStatementsWithFunctionCall(stmtsRange, extractedMethodName);
    }

    bool selectionOverlapsWithStmt(const clang::Stmt& stmt)
    {
        return selection.overlapsWith(sourceExtractor.getOffsetRange(sourceExtractor.getCorrectSourceRange(stmt)));
    }

    clang::ConstStmtRange findStatementsTouchingSelection(const clang::FunctionDecl& func)
    {
        auto body = func.getBody();
        auto begin =
            std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return selectionOverlapsWithStmt(*s); });
        auto end =
            std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !selectionOverlapsWithStmt(*s); });
        return {begin, end};
    }

    void printExtractedFunction(clang::SourceLocation at, const std::string& name, clang::SourceRange stmtsRange)
    {
        printFunction(name, sourceExtractor.getSource(stmtsRange), sourceExtractor.getOffset(at));
    }

    void printFunction(const std::string& name, const std::string& body, unsigned offset)
    {
        std::ostringstream os;
        os << "void " << name << "()\n{\n    " << body << "\n}\n";
        sourceOperations.insertTextAt(os.str(), offset);
    }

    void replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName)
    {
        replaceRangeWith(stmtsRange, functionName + "();");
    }

    void replaceRangeWith(clang::SourceRange without, std::string replace)
    {
        auto begin = sourceExtractor.getOffset(without.getBegin());
        auto end = sourceExtractor.getOffset(without.getEnd());
        sourceOperations.removeTextInRange(begin, end);
        sourceOperations.insertTextAt(replace, begin);
    }
};

class MethodExtractorUnitHandler : public clang::ASTConsumer
{
public:
    MethodExtractorUnitHandler(const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx)
    {
        SourceExtractor sourceExtractor(ctx.getSourceManager());
        MethodExtractor extractor(sourceExtractor, extractedMethodName, selection, sourceOperations);
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
    MethodExtractorFactoryFactory(const std::string& extractedMethodName, SourceSelection selection, TextOperationApplier& sourceOperations)
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

void performFrontendActionForFile(clang::tooling::FrontendActionFactory& actionFactory, std::string sourceFilename)
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

void extractMethodInFile(const std::string& methodName, SourceSelection selection, const std::string& filename)
{
    TextOperationApplier sourceOperations;
    MethodExtractorFactoryFactory extractorFactoryFactory(methodName, selection, sourceOperations);
    performFrontendActionForFile(extractorFactoryFactory, filename);
    applySourceOperationsToFile(sourceOperations, filename);
}