#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "MethodExtractorVisitor.hpp"
#include "NaiveLocalVariableLocator.hpp"
#include <clang/AST/ASTContext.h>

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    NaiveStatementLocator stmtLocator(sourceExtractor, selection);
    NaiveLocalVariableLocator localVariableLocator;
    DelayedMethodExtractor extractor(sourceExtractor, sourceOperations, printer, localVariableLocator);
    MethodExtractorVisitor visitor(extractedMethodName, extractor, stmtLocator);
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
}
