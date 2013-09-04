#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "MethodExtractorVisitor.hpp"
#include <clang/AST/ASTContext.h>

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    NaiveStatementLocator locator(sourceExtractor, selection);
    DelayedMethodExtractor extractor(sourceExtractor, sourceOperations, printer);
    MethodExtractorVisitor visitor(extractedMethodName, extractor, locator);
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}
