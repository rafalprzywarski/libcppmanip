#ifndef CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
#define CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
#include <cppmanip/SourceReplacement.hpp>
#include <cppmanip/SourceSelection.hpp>
#include <memory>

namespace cppmanip
{

class FunctionExtractor
{
public:
    virtual ~FunctionExtractor() { }
    virtual boundary::SourceReplacements extractFunctionInFile() = 0;
};

typedef std::shared_ptr<FunctionExtractor> FunctionExtractorPtr;

}
#endif // CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
