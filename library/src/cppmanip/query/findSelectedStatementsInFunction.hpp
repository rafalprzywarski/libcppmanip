#ifndef CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#define CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#include "IsStatementSelected.hpp"
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Stmt.h>
#include <cppmanip/ast/Function.hpp>

namespace cppmanip
{
namespace query
{

ast::ScopedStatementRange findSelectedStatementsInFunction(
    const ast::Function& decl, IsStatementSelected isSelected);

}
}
#endif // CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
