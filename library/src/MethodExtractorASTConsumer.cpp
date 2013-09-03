#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include <clang/AST/ASTContext.h>

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    MethodExtractor extractor(sourceExtractor, extractedMethodName, selection, sourceOperations);
    extractor.TraverseDecl(ctx.getTranslationUnitDecl());
}
