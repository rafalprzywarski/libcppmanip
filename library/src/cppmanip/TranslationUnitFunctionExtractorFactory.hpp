#ifndef CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#define CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#include "text/TextModifier.hpp"
#include "clangutil/HandleTranslationUnit.hpp"
#include "ast/SourceOffset.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractorFactory
{
public:
    clangutil::HandleTranslationUnit createFunctionExtractor(
        const std::string& extractedMethodName, ast::SourceOffsetRange selection, cppmanip::text::OffsetBasedTextModifier& sourceOperations);
};

}
#endif // CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
