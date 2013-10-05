#include "findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{
namespace query
{

ast::LocalVariables findVariablesDeclaredByAndUsedAfterStmts(ast::StatementRange stmts, const ast::Statements& scope)
{
    using namespace boost::adaptors;
    std::unordered_set<ast::LocalVariablePtr> declared;
    for (auto s : stmts)
        declared.insert(s->getDeclaredVariables().begin(), s->getDeclaredVariables().end());
    std::unordered_set<ast::LocalVariablePtr> used;
    for (auto s : ast::StatementRange(end(stmts), end(scope)))
        used.insert(s->getUsedLocalVariables().begin(), s->getUsedLocalVariables().end());

    ast::LocalVariables result;
    auto isDeclared = [&](ast::LocalVariablePtr d) { return declared.count(d) != 0; };
    boost::push_back(result, used | filtered(isDeclared));
    return result;
}

}
}
