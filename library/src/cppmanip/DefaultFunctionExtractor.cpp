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

namespace
{

ast::SourceOffsetRange getRange(ast::StatementRange stmts)
{
    return { stmts.front()->getRange().getFrom(), stmts.back()->getRange().getTo() };
}

}

boundary::SourceReplacements DefaultFunctionExtractor::extractFunctionFromSelectionInFile(
    const std::string& functionName, boundary::SourceSelection selection, const std::string& filename)
{
    auto selected = stmtLocator->getSelectedFunctionAndStmts(selection);
    validator->validateStatements(functionName, selected.stmts);
    auto replacementFunction = printer->printFunctionFromStmts(functionName, selected.stmts.getRange());
    return replacer->generateReplacements(replacementFunction, selected.function->getDefinitionOffset(), getRange(selected.stmts.getRange()));
}

}
