#include "ClangFunctionExtractor.hpp"
#include "text/OffsetConverter.hpp"
#include "io/TextFileOps.hpp"
#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "clangutil/runTranslationUnitHandlerOnFile.hpp"
#include "text/TextReplacementRecorder.hpp"

namespace cppmanip
{
boundary::SourceReplacements ClangFunctionExtractor::extractFunctionInFile()
{
    runTranslationUnitHandler();
    return getReplacements();
}

}

