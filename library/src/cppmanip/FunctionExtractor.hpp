#ifndef CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
#define CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
#include <cppmanip/SourceReplacement.hpp>
#include <cppmanip/SourceSelection.hpp>

namespace cppmanip
{

class FunctionExtractor
{
public:
    virtual ~FunctionExtractor() { }
    virtual SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename) = 0;
};

}
#endif // CPPMANIP_922CAAE1ABCA4963BD33D5ECDE093836_HPP
