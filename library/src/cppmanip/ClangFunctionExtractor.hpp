#ifndef CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
#define CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
#include "FunctionExtractor.hpp"
#include "text/TextModifier.hpp"
#include "clangutil/TranslationUnitHandler.hpp"

namespace cppmanip
{

class ClangFunctionExtractor : public FunctionExtractor
{
public:
    ClangFunctionExtractor(text::OffsetBasedTextModifier& textModifier, const std::string& filename,
        clangutil::TranslationUnitHandlerPtr translationUnitHandler
    )
        : textModifier(textModifier), filename(filename), translationUnitHandler(translationUnitHandler) { }
    virtual SourceReplacements extractFunctionInFile();
private:
    text::OffsetBasedTextModifier& textModifier;
    std::string filename;
    clangutil::TranslationUnitHandlerPtr translationUnitHandler;
};

}
#endif // CPPMANIP_96167AD230584C3EAA5F0BF303AB37C9_HPP
