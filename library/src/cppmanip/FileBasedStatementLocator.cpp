#include "FileBasedStatementLocator.hpp"
#include "math/PositionRange.hpp"

namespace cppmanip
{

namespace
{

bool overlaps(const ast::Statement& s, ast::SourceOffsetRange selection)
{
    math::PositionRange<ast::SourceOffset> sr(s.getRange().getFrom(), s.getRange().getTo()), sel(selection.getFrom(), selection.getTo());
    return sel.overlapsWith(sr);
}

}

cppmanip::StatementLocator::FunctionAndStmts FileBasedStatementLocator::getSelectedFunctionAndStmts(cppmanip::boundary::SourceSelection selection)
{
    ast::SourceOffsetRange offsetSelection{ getOffsetFromLocation(selection.from), getOffsetFromLocation(selection.to) };
    auto originalFunction = astGateway->getFunctionInSelectionFromFile(offsetSelection, filename);
    auto selected = findSelectedStatementsInFunction(*originalFunction, [=](ast::StatementPtr stmt) { return overlaps(*stmt, offsetSelection); });
    return { originalFunction, selected.getRange() };
}

}
