#ifndef CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
#define CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
#include <memory>
#include "FunctionExtractor.hpp"

namespace cppmanip
{

class ClangFunctionExtractorFactory
{
public:
    std::shared_ptr<FunctionExtractor> createFunctionExtractor();
};

}
#endif // CPPMANIP_D11663A8E4184CB385DC07696195FEFF_HPP
