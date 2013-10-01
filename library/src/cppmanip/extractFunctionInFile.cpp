#include <cppmanip/extractFunctionInFile.hpp>
#include "ClangFunctionExtractorFactory.hpp"

namespace cppmanip
{
namespace boundary
{

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename)
{
    auto extractor = ClangFunctionExtractorFactory().createFunctionExtractor(functionName, selection, filename);
    return extractor->extractFunctionInFile();
}

}
}
