#ifndef CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
#define CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
#include "FunctionExtractor.hpp"
#include "TextModifier.hpp"

namespace cppmanip
{

class ClangFunctionExtractor : public FunctionExtractor
{
public:
    ClangFunctionExtractor(OffsetBasedTextModifier& textModifier)
        : textModifier(textModifier) { }
    virtual SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);
private:
    OffsetBasedTextModifier& textModifier;
};

}
#endif // CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
