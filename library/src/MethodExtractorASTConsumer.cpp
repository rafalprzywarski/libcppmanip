#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include <clang/AST/ASTContext.h>

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    MethodExtractor extractor(sourceExtractor, extractedMethodName, selection, sourceOperations, printer);
    extractor.TraverseDecl(ctx.getTranslationUnitDecl());
}
