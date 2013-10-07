#include "DefaultFunctionExtractor.hpp"
#include "query/findLocalVariablesRequiredForStmts.hpp"
#include "query/findSelectedStatementsInFunction.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "format/printFunction.hpp"
#include "text/StrictOperationRecorder.hpp"
#include "text/OffsetConverter.hpp"
#include "text/SourceLocationConverter.hpp"
#include "text/convertReplacements.hpp"
#include "io/TextFileOps.hpp"
#include "clangutil/AstGateway.hpp"
#include "FileBasedStatementLocator.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace cppmanip
{

void defineFunction(const std::string& definition, ast::FunctionPtr originalFunction, text::OffsetBasedOperationRecorder& recorder)
{
    recorder.insertTextAt(definition, originalFunction->getDefinitionOffset());
}

ast::SourceOffsetRange getRange(ast::StatementRange stmts)
{
    return { stmts.front()->getRange().getFrom(), stmts.back()->getRange().getTo() };
}

void replaceStmtsWithCall(ast::StatementRange stmts, const std::string& call, text::OffsetBasedOperationRecorder& recorder)
{
    auto range = getRange(stmts);
    recorder.removeTextInRange(range.getFrom(), range.getTo());
    recorder.insertTextAt(call, range.getFrom());
}

boundary::SourceReplacements generateReplacements(format::ReplacementFunction replacementFunction, StatementLocator::FunctionAndStmts selected, const std::string& filename)
{
    text::OffsetBasedStrictOperationRecorder recorder;
    defineFunction(replacementFunction.definition, selected.function, recorder);
    replaceStmtsWithCall(selected.stmts, replacementFunction.call, recorder);
    text::OffsetConverter offsetCoverter(io::loadTextFromFile(filename));
    return text::convertReplacements(recorder.getReplacements(), [&](unsigned offset) { return offsetCoverter.getLocationFromOffset(offset); });
}

boundary::SourceReplacements DefaultFunctionExtractor::extractFunctionFromSelectionInFile(
    const std::string& functionName, boundary::SourceSelection selection, const std::string& filename)
{
    auto selected = stmtLocator->getSelectedFunctionAndStmts(selection);
    validator->validateStatements(functionName, selected.stmts, selected.function);
    auto replacementFunction = functionPrinter->printFunctionFromStmts(functionName, selected.stmts);
    return generateReplacements(replacementFunction, selected, filename);
}

}
