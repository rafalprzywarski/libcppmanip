#include "ClangFunctionExtractorFactory.hpp"
#include "ClangFunctionExtractor.hpp"

namespace cppmanip
{

std::shared_ptr<FunctionExtractor> ClangFunctionExtractorFactory::createFunctionExtractor()
{
    return std::make_shared<ClangFunctionExtractor>();
}

}

