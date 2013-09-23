#ifndef CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#define CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#include "clangutil/TranslationUnitHandler.hpp"
#include "text/TextModifier.hpp"
#include "LocationRange.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractorFactory
{
public:
    clangutil::TranslationUnitHandlerPtr createFunctionExtractor(
        const std::string& extractedMethodName, LocationRange selection, cppmanip::text::OffsetBasedTextModifier& sourceOperations);
};

}
#endif // CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
