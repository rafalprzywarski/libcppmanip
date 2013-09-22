#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "MethodExtractorVisitor.hpp"
#include "NaiveLocalVariableLocator.hpp"
#include "DefaultClangFunctionLocatorFactory.hpp"
#include "DefaultClangFunctionLocator.hpp"
#include "makeWithDependencies.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    auto functionExtractor = TranslationUnitFunctionExtractorFactory().createFunctionExtractor(extractedMethodName, selection, sourceOperations);
    functionExtractor->handleTranslationUnit(ctx);
}

}
