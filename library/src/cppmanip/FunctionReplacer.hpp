#ifndef CPPMANIP_719B77A3C8DA418D9F7B56A6F7CB6CF7_HPP
#define CPPMANIP_719B77A3C8DA418D9F7B56A6F7CB6CF7_HPP
#include <cppmanip/boundary/SourceReplacement.hpp>
#include "format/ReplacementFunctionPrinter.hpp"

namespace cppmanip
{

class FunctionReplacer
{
public:
    virtual ~FunctionReplacer() { }
    virtual boundary::SourceReplacements generateReplacements(
        format::ReplacementFunction replacementFunction, ast::SourceOffset definitionOffset, ast::SourceOffsetRange stmts) = 0;
};

typedef std::shared_ptr<FunctionReplacer> FunctionReplacerPtr;

}
#endif // CPPMANIP_719B77A3C8DA418D9F7B56A6F7CB6CF7_HPP
