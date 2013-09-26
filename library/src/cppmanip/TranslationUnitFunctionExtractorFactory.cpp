#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "legacy/PrettyFunctionPrinter.hpp"
#include "legacy/NaiveLocalVariableLocator.hpp"
#include "legacy/DelayedMethodExtractor.hpp"
#include "TranslationUnitFunctionExtractor.hpp"
#include "getFunctionFromAstInSelection.hpp"
#include "DefaultStatementLocator.hpp"
#include "getStmtLocationRange.hpp"

namespace cppmanip
{

clangutil::TranslationUnitHandlerPtr TranslationUnitFunctionExtractorFactory::createFunctionExtractor(
    const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
{
    using std::bind;
    using namespace std::placeholders;
    struct WithDeps
    {
        legacy::PrettyFunctionPrinter printer;
        DefaultStatementLocator stmtLocator;
        legacy::NaiveLocalVariableLocator localVariableLocator;
        legacy::DelayedMethodExtractor stmtExtractor;
        TranslationUnitFunctionExtractor functionExtractor;
        WithDeps(const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
            : stmtLocator(getStmtLocationRange, selection),
            stmtExtractor(sourceOperations, printer, localVariableLocator, extractedMethodName),
            functionExtractor(bind(getFunctionFromAstInSelection, _1, selection), stmtLocator, stmtExtractor) { }
    };
    auto withDeps = std::make_shared<WithDeps>(extractedMethodName, selection, sourceOperations);
    return std::shared_ptr<clangutil::TranslationUnitHandler>(withDeps, &withDeps->functionExtractor);
}

}

