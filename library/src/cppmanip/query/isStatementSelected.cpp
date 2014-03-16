#include "isStatementSelected.hpp"
#include <cppmanip/math/PositionRange.hpp>
#include <boost/range/algorithm/find_if.hpp>

namespace cppmanip
{
namespace query
{
namespace
{

bool overlaps(ast::SourceOffsetRange left, ast::SourceOffsetRange right)
{
    return math::PositionRange<ast::SourceOffset>(left.getFrom(), left.getTo()).overlapsWith({right.getFrom(), right.getTo()});
}

bool overlapsSpecificRanges(ast::StatementPtr stmt, ast::SourceOffsetRange selection)
{
    return !boost::empty(
        boost::find_if<boost::return_found_end>(
            stmt->getSpecificRanges(), [=](ast::SourceOffsetRange s) { return overlaps(s, selection); }));
}

bool overlapsRange(ast::StatementPtr stmt, ast::SourceOffsetRange selection)
{
    return overlaps(stmt->getRange(), selection);
}

}

StatementSelected isStatementSelected(ast::StatementPtr stmt, ast::SourceOffsetRange selection)
{
    if (overlapsSpecificRanges(stmt, selection))
        return StatementSelected::EXPLICITLY;
    if (overlapsRange(stmt, selection))
        return StatementSelected::IMPLICITLY;
    return StatementSelected::NO;
}

}
}

