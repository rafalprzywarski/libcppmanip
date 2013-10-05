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

ast::LocalVariables findLocalVariablesRequiredForStmts(ast::StatementRange stmts)
{
    std::unordered_set<ast::LocalVariablePtr> required;
    for (auto s : stmts)
        required.insert(s->getUsedLocalVariables().begin(), s->getUsedLocalVariables().end());
    for (auto s : stmts)
        for (auto v : s->getDeclaredVariables())
            required.erase(v);
    return {required.begin(), required.end()};
}

}
}
