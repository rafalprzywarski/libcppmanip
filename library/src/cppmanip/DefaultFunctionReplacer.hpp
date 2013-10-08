#ifndef CPPMANIP_926BA0577B064302BAB21F6A6EE2E715_HPP
#define CPPMANIP_926BA0577B064302BAB21F6A6EE2E715_HPP
#include "FunctionReplacer.hpp"
#include "text/OperationRecorder.hpp"

namespace cppmanip
{

class DefaultFunctionReplacer : public FunctionReplacer
{
public:
    typedef std::function<text::OffsetBasedOperationRecorderPtr()> OperationRecorderFactory;
    typedef std::function<boundary::SourceReplacements(const text::Replacements<ast::SourceOffset>&)> ConvertReplacements;
    DefaultFunctionReplacer(OperationRecorderFactory createOperationRecoder, ConvertReplacements convertReplacements)
        : createOperationRecoder(createOperationRecoder), convertReplacements(convertReplacements) { }
    virtual boundary::SourceReplacements generateReplacements(
        format::ReplacementFunction replacementFunction, ast::SourceOffset definitionOffset, ast::SourceOffsetRange stmts);
private:
    OperationRecorderFactory createOperationRecoder;
    ConvertReplacements convertReplacements;
    void defineFunction(const std::string& definition, ast::SourceOffset offset, text::OffsetBasedOperationRecorder& recorder);
    void replaceStmtsWithCall(ast::SourceOffsetRange stmts, const std::string& call, text::OffsetBasedOperationRecorder& recorder);
};

}

#endif // CPPMANIP_926BA0577B064302BAB21F6A6EE2E715_HPP
