#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "legacy/DelayedFunctionExtractor.hpp"
#include "TranslationUnitFunctionExtractor.hpp"
#include "query/getFunctionFromAstInSelection.hpp"
#include "query/findStatementsInFunctionOverlappingSelection.hpp"
#include "query/getStmtLocationRange.hpp"
#include "query/findLocalVariablesRequiredForStmts.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "format/printFunction.hpp"

namespace cppmanip
{

clangutil::HandleTranslationUnit TranslationUnitFunctionExtractorFactory::createFunctionExtractor(
    const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
{
    using std::bind;
    using namespace std::placeholders;
    struct WithDeps
    {
        TranslationUnitFunctionExtractor functionExtractor;
        WithDeps(const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
            : functionExtractor(
                bind(query::getFunctionFromAstInSelection, _1, selection),
                bind(query::findStatementsInFunctionOverlappingSelection, _1, selection, query::getStmtLocationRange),
                [&](clang::ASTContext& ) {
                    return std::make_shared<legacy::DelayedFunctionExtractor>(
                        sourceOperations, format::printFunctionCall, format::printFunctionDefinition,
                        query::findLocalVariablesRequiredForStmts,
                        query::findVariablesDeclaredByAndUsedAfterStmts, extractedMethodName);
                }) { }
    };
    auto withDeps = std::make_shared<WithDeps>(extractedMethodName, selection, sourceOperations);
    std::shared_ptr<TranslationUnitFunctionExtractor> handler{withDeps, &withDeps->functionExtractor};
    return [=](clang::ASTContext& ctx){ handler->handleTranslationUnit(ctx); };
}

}

