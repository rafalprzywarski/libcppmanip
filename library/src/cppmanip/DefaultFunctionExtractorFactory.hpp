#ifndef CPPMANIP_2F97FBCBD6F14246B41E2EF3E6F1C692_HPP
#define CPPMANIP_2F97FBCBD6F14246B41E2EF3E6F1C692_HPP
#include "DefaultFunctionExtractor.hpp"

namespace cppmanip
{

class DefaultFunctionExtractorFactory
{
public:
    DefaultFunctionExtractorPtr createForFile(const std::string& filename);
};

}
#endif // CPPMANIP_2F97FBCBD6F14246B41E2EF3E6F1C692_HPP
