#ifndef CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
#define CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
#include <memory>
#include "FunctionExtractor.hpp"

namespace cppmanip
{

class ClangFunctionExtractorFactory
{
public:
    FunctionExtractorPtr createFunctionExtractor(const std::string& functionName, SourceSelection selection, const std::string& filename);
};

}
#endif // CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
