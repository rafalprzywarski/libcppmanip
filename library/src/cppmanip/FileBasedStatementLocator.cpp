#include "FileBasedStatementLocator.hpp"
#include "math/PositionRange.hpp"

namespace cppmanip
{

cppmanip::StatementLocator::FunctionAndStmts FileBasedStatementLocator::getSelectedFunctionAndStmts(cppmanip::boundary::SourceSelection selection)
{
    ast::SourceOffsetRange offsetSelection{ getOffsetFromLocation(selection.from), getOffsetFromLocation(selection.to) };
    auto originalFunction = astGateway->getFunctionInSelectionFromFile(offsetSelection, filename);
    auto selected = findSelectedStatementsInFunction(*originalFunction, [=](ast::StatementPtr stmt) { return isStatementSelected(stmt, offsetSelection); });
    return { originalFunction, selected };
}

}
