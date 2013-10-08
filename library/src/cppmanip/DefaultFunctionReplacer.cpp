#include "DefaultFunctionReplacer.hpp"

namespace cppmanip
{
void DefaultFunctionReplacer::defineFunction(const std::string& definition, ast::SourceOffset offset, text::OffsetBasedOperationRecorder& recorder)
{
    recorder.insertTextAt(definition, offset);
}

void DefaultFunctionReplacer::replaceStmtsWithCall(ast::SourceOffsetRange stmts, const std::string& call, text::OffsetBasedOperationRecorder& recorder)
{
    recorder.removeTextInRange(stmts.getFrom(), stmts.getTo());
    recorder.insertTextAt(call, stmts.getFrom());
}

boundary::SourceReplacements DefaultFunctionReplacer::generateReplacements(
    format::ReplacementFunction replacementFunction, ast::SourceOffset definitionOffset, ast::SourceOffsetRange stmts)
{
    auto recorder = createOperationRecoder();
    defineFunction(replacementFunction.definition, definitionOffset, *recorder);
    replaceStmtsWithCall(stmts, replacementFunction.call, *recorder);
    return convertReplacements(recorder->getReplacements());
}

}

