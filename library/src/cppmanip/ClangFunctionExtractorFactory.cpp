#include "ClangFunctionExtractorFactory.hpp"
#include "ClangFunctionExtractor.hpp"
#include "text/TextOperationApplier.hpp"
#include "makeWithDependencies.hpp"
#include "LocationRange.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"

namespace cppmanip
{

FunctionExtractorPtr ClangFunctionExtractorFactory::createFunctionExtractor(
    const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    struct WithDeps
    {
        text::OffsetBasedTextOperationApplier textModifier;
        ClangFunctionExtractor extractor;
        WithDeps(const std::string& functionName, SourceSelection selection, const std::string& filename)
            : extractor(textModifier, filename,
                        TranslationUnitFunctionExtractorFactory().createFunctionExtractor(
                            functionName, LocationRange(selection.from, selection.to), textModifier)) { }
    };
    auto withDeps = std::make_shared<WithDeps>(functionName, selection, filename);
    return std::shared_ptr<FunctionExtractor>(withDeps, &withDeps->extractor);
}

}

