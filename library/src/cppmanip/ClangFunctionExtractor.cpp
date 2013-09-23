#include "ClangFunctionExtractor.hpp"
#include "OffsetConverter.hpp"
#include "io/TextFileOps.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "clangutil/runTranslationUnitHandlerOnFile.hpp"
#include "text/TextReplacementRecorder.hpp"

namespace cppmanip
{

SourceReplacements recordReplacements(const text::OffsetBasedTextModifier& sourceOperations, const std::string& source)
{
    OffsetConverter offsetCoverter(source);
    text::TextReplacementRecorder recorder(std::bind(&OffsetConverter::getLocationFromOffset, &offsetCoverter, std::placeholders::_1));
    sourceOperations.apply(recorder);
    return recorder.getReplacements();
}

SourceReplacements ClangFunctionExtractor::extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    std::string source = io::loadTextFromFile(filename);
    auto functionExtractor = TranslationUnitFunctionExtractorFactory().createFunctionExtractor(functionName, LocationRange(selection.from, selection.to), textModifier);
    clangutil::runTranslationUnitHandlerOnFile(functionExtractor, filename);
    return recordReplacements(textModifier, source);
}

}

