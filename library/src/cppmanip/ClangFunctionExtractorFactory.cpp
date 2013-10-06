#include "ClangFunctionExtractorFactory.hpp"
#include "ClangFunctionExtractor.hpp"
#include "text/TextOperationApplier.hpp"
#include "text/OffsetConverter.hpp"
#include "text/TextReplacementRecorder.hpp"
#include "text/SourceLocationConverter.hpp"
#include "makeWithDependencies.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "io/TextFileOps.hpp"
#include "clangutil/runTranslationUnitHandlerOnFile.hpp"

namespace cppmanip
{

namespace
{
boundary::SourceReplacements recordReplacements(const text::OffsetBasedTextModifier& sourceOperations, const std::string& filename)
{
    text::OffsetConverter offsetCoverter(io::loadTextFromFile(filename));
    text::TextReplacementRecorder recorder([&](unsigned offset) { return offsetCoverter.getLocationFromOffset(offset); });
    sourceOperations.apply(recorder);
    return recorder.getReplacements();
}
}

FunctionExtractorPtr ClangFunctionExtractorFactory::createFunctionExtractor(
    const std::string& functionName, boundary::SourceSelection selection, const std::string& filename)
{
    struct WithDeps
    {
        text::OffsetBasedTextOperationApplier textModifier;
        ClangFunctionExtractor extractor;
        WithDeps(const std::string& functionName, ast::SourceOffsetRange selection, const std::string& filename)
            : extractor(
                [&]{ clangutil::runTranslationUnitHandlerOnFile(
                    TranslationUnitFunctionExtractorFactory().createFunctionExtractor(
                        functionName, selection, textModifier), filename); },
                [&]{ return recordReplacements(textModifier, filename); }) { }
    };
    text::SourceLocationConverter conv{io::loadTextFromFile(filename)};
    ast::SourceOffsetRange range{ conv.getOffsetFromLocation(selection.from), conv.getOffsetFromLocation(selection.to) };
    auto withDeps = std::make_shared<WithDeps>(functionName, range, filename);
    return std::shared_ptr<FunctionExtractor>(withDeps, &withDeps->extractor);
}

}

