#include "findSelectedStatementsInFunction.hpp"
#include <clang/AST/ASTContext.h>
#include <boost/range/algorithm.hpp>

namespace cppmanip
{
namespace query
{
namespace
{

ast::ScopedStatementRange findSelectedStatements(ast::StatementRange stmts, IsStatementSelected isSelected)
{
    using namespace boost;
    return { stmts, find_if<return_begin_found>(
        find_if<return_found_end>(stmts, isSelected), [&](const ast::StatementPtr& s) { return !isSelected(s); }) };
}

}

ast::ScopedStatementRange findSelectedStatementsInFunction(
    const ast::Function& decl, IsStatementSelected isSelected)
{
    auto found = findSelectedStatements(*decl.getStatements(), isSelected);
    if (found.getRange().size() != 1 || found.getRange().front()->getChildren()->empty())
        return found;
    return findSelectedStatements(*found.getRange().front()->getChildren(), isSelected);
}

}
}
