#include "findSelectedStatementsInFunction.hpp"
#include <clang/AST/ASTContext.h>
#include <boost/range/algorithm.hpp>
#include <boost/optional.hpp>

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
        find_if<return_found_end>(stmts, [&](const ast::StatementPtr& s) { return isSelected(s) != StatementSelected::NO; }),
        [&](const ast::StatementPtr& s) { return isSelected(s) == StatementSelected::NO; }) };
}

bool shouldVisitChildren(ast::ScopedStatementRange parent, IsStatementSelected isSelected)
{
    if (parent.getRange().size() != 1)
        return false;
    auto onlyParent = parent.getRange().front();
    return !onlyParent->getChildGroups().empty() && isSelected(onlyParent) != StatementSelected::EXPLICITLY;
}

boost::optional<ast::ScopedStatementRange> findSelectedChildren(ast::StatementGroups groups, IsStatementSelected isSelected)
{
    std::vector<ast::ScopedStatementRange> selectedInGroups;
    for (auto children : groups)
    {
        auto selected = findSelectedStatements(*children, isSelected);
        if (!selected.getRange().empty())
            selectedInGroups.push_back(selected);
    }
    if (selectedInGroups.size() != 1)
        return boost::none;
    return selectedInGroups.front();
}

}

ast::ScopedStatementRange findSelectedStatementsInFunction(
    const ast::Function& decl, IsStatementSelected isSelected)
{
    auto parent = findSelectedStatements(*decl.getStatements(), isSelected);
    if (!shouldVisitChildren(parent, isSelected))
        return parent;
    if (auto children = findSelectedChildren(parent.getRange().front()->getChildGroups(), isSelected))
        return *children;
    return parent;
}

}
}
