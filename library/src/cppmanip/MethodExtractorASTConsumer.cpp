#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "MethodExtractorVisitor.hpp"
#include "NaiveLocalVariableLocator.hpp"
#include "DefaultClangFunctionLocatorFactory.hpp"
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    DefaultClangFunctionLocatorFactory functionLocatorFactory;

    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    NaiveStatementLocator stmtLocator(selection);
    NaiveLocalVariableLocator localVariableLocator;
    DelayedMethodExtractor extractor(sourceExtractor, sourceOperations, printer, localVariableLocator);
    auto locator = functionLocatorFactory.createFunctionLocator(selection);
    auto& f = locator->getFunction(ctx);
    auto stmts = stmtLocator.findStatementsInFunction(f);
    extractor.extractStatmentsFromFunctionIntoNewFunction(stmts, f, extractedMethodName);
}

}
