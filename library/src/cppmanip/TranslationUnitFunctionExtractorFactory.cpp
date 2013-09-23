#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "DefaultClangFunctionLocator.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "NaiveLocalVariableLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "TranslationUnitFunctionExtractor.hpp"

namespace cppmanip
{

clangutil::TranslationUnitHandlerPtr TranslationUnitFunctionExtractorFactory::createFunctionExtractor(
    const std::string& extractedMethodName, OffsetRange selection, text::OffsetBasedTextModifier& sourceOperations)
{
    struct WithDeps
    {
        DefaultClangFunctionLocator functionLocator;
        PrettyFunctionPrinter printer;
        NaiveStatementLocator stmtLocator;
        NaiveLocalVariableLocator localVariableLocator;
        DelayedMethodExtractor stmtExtractor;
        TranslationUnitFunctionExtractor functionExtractor;
        WithDeps(const std::string& extractedMethodName, OffsetRange selection, text::OffsetBasedTextModifier& sourceOperations)
            : functionLocator(selection), stmtLocator(selection), stmtExtractor(sourceOperations, printer, localVariableLocator, extractedMethodName),
            functionExtractor(functionLocator, stmtLocator, stmtExtractor) { }
    };
    auto withDeps = std::make_shared<WithDeps>(extractedMethodName, selection, sourceOperations);
    return std::shared_ptr<clangutil::TranslationUnitHandler>(withDeps, &withDeps->functionExtractor);
}

}

