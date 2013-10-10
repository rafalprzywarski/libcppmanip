#include "findLocalVariablesRequiredForStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{
namespace query
{

namespace
{

std::unordered_set<ast::LocalVariablePtr> getUsedVariables(ast::StatementRange stmts)
{
    std::unordered_set<ast::LocalVariablePtr> used;
    for (auto s : stmts)
        used.insert(s->getUsedLocalVariables().begin(), s->getUsedLocalVariables().end());
    return used;
}

std::unordered_set<ast::LocalVariablePtr> removeDeclaredVariables(std::unordered_set<ast::LocalVariablePtr>&& used, ast::StatementRange stmts)
{
    for (auto s : stmts)
        for (auto v : s->getDeclaredVariables())
            used.erase(v);
    return used;
}

ast::LocalVariables asList(std::unordered_set<ast::LocalVariablePtr>&& vars)
{
    return {vars.begin(), vars.end()};
}

bool byOffset(ast::LocalVariablePtr left, ast::LocalVariablePtr right)
{
    return left->getDeclarationOffset() < right->getDeclarationOffset();
}

}

ast::LocalVariables findLocalVariablesRequiredForStmts(ast::StatementRange stmts)
{
    ast::LocalVariables required = asList(removeDeclaredVariables(getUsedVariables(stmts), stmts));
    boost::sort(required, byOffset);
    return required;
}

}
}
