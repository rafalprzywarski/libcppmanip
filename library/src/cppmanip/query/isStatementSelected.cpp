#include "isStatementSelected.hpp"
#include <cppmanip/math/PositionRange.hpp>

namespace cppmanip
{
namespace query
{

StatementSelected isStatementSelected(ast::StatementPtr stmt, ast::SourceOffsetRange selection)
{
    auto sr = stmt->getRange();
    math::PositionRange<ast::SourceOffset> r1{sr.getFrom(), sr.getTo()}, r2{selection.getFrom(), selection.getTo()};
    if (r1.overlapsWith(r2))
        return StatementSelected::IMPLICITLY;
    return StatementSelected::NO;
}

}
}

