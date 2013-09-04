#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include <clang/AST/ASTContext.h>

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    NaiveStatementLocator locator(sourceExtractor, selection);
    MethodExtractor extractor(sourceExtractor, extractedMethodName, locator, sourceOperations, printer);
    extractor.TraverseDecl(ctx.getTranslationUnitDecl());
}
