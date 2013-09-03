#include "CppManip.hpp"
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Lex/Lexer.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "TextOperationApplier.hpp"
#include "OffsetRange.hpp"
#include "TextFileOps.hpp"
#include "ClangToolArgsBuilder.hpp"
#include "MethodExtractor.hpp"
#include <functional>

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

class MethodExtractorFrontendAction: public clang::ASTFrontendAction
{
public:
    MethodExtractorFrontendAction(const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
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

class MethodExtractorFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    MethodExtractorFrontendActionFactory(const std::string& extractedMethodName, SourceSelection selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection.from, selection.to), sourceOperations(sourceOperations) { }
    virtual clang::FrontendAction* create()
    {
        return new MethodExtractorFrontendAction(extractedMethodName, selection, sourceOperations);
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
    MethodExtractorFrontendActionFactory factory(methodName, selection, sourceOperations);
    performFrontendActionForFile(factory, filename);
    applySourceOperationsToFile(sourceOperations, filename);
}