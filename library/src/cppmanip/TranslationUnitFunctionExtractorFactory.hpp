#ifndef CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#define CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
#include "TranslationUnitHandler.hpp"
#include "OffsetRange.hpp"
#include "TextModifier.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractorFactory
{
public:
    std::shared_ptr<TranslationUnitHandler> createFunctionExtractor(
        const std::string& extractedMethodName, OffsetRange selection, OffsetBasedTextModifier& sourceOperations);
};

}
#endif // CPPMANIP_BFCEF719CED24F62880950DB1CFBA8A0_HPP
