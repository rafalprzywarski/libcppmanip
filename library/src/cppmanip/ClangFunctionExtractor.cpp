#include "ClangFunctionExtractor.hpp"
#include "SourceLocationConverter.hpp"
#include "OffsetConverter.hpp"
#include "TextReplacementRecorder.hpp"
#include "io/TextFileOps.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "clangutil/runTranslationUnitHandlerOnFile.hpp"

namespace cppmanip
{

OffsetRange getSourceRange(SourceSelection selection, const std::string& source)
{
    SourceLocationConverter sourceLocationConverter(source);
    return OffsetRange(
        sourceLocationConverter.getOffsetFromLocation(selection.from),
        sourceLocationConverter.getOffsetFromLocation(selection.to));
}

SourceReplacements recordReplacements(const OffsetBasedTextModifier& sourceOperations, const std::string& source)
{
    OffsetConverter offsetCoverter(source);
    TextReplacementRecorder recorder(std::bind(&OffsetConverter::getLocationFromOffset, &offsetCoverter, std::placeholders::_1));
    sourceOperations.apply(recorder);
    return recorder.getReplacements();
}

SourceReplacements ClangFunctionExtractor::extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    std::string source = io::loadTextFromFile(filename);
    auto functionExtractor = TranslationUnitFunctionExtractorFactory().createFunctionExtractor(functionName, getSourceRange(selection, source), textModifier);
    clangutil::runTranslationUnitHandlerOnFile(functionExtractor, filename);
    return recordReplacements(textModifier, source);
}

}

