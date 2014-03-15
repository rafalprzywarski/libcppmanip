#include <cppmanip/boundary/extractFunctionInFile.hpp>
#include "DefaultFunctionExtractor.hpp"
#include "DefaultFunctionExtractorFactory.hpp"

namespace cppmanip
{
namespace boundary
{

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    auto extractor = DefaultFunctionExtractorFactory().createForFile(filename);
    return extractor->extractFunctionFromSelection(functionName, selection);
}

}
}
