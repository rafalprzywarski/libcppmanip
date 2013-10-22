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

bool shouldVisitChildren(ast::ScopedStatementRange parent)
{
    return parent.getRange().size() == 1 && !parent.getRange().front()->getChildGroups().empty();
}

ast::ScopedStatementRange findSelectedChildren(ast::StatementGroups groups, IsStatementSelected isSelected)
{
    for (auto stmts : groups)
    {
        auto found = findSelectedStatements(*stmts, isSelected);
        if (!found.getRange().empty())
            return found;
    }
    throw std::logic_error("not implemented");
}

}

ast::ScopedStatementRange findSelectedStatementsInFunction(
    const ast::Function& decl, IsStatementSelected isSelected)
{
    auto parent = findSelectedStatements(*decl.getStatements(), isSelected);
    if (!shouldVisitChildren(parent))
        return parent;
    return findSelectedChildren(parent.getRange().front()->getChildGroups(), isSelected);
}

}
}
