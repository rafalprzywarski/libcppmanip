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
#include "MethodExtractorASTConsumer.hpp"
#include "MethodExtractorFrontendAction.hpp"
#include "MethodExtractorFrontendActionFactory.hpp"
#include <functional>

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