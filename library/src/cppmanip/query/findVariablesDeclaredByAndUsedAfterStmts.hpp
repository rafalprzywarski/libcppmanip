#ifndef FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#define FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#include <cppmanip/ast/LocalVariable.hpp>
#include <cppmanip/ast/Statement.hpp>
#include <cppmanip/ast/Function.hpp>

namespace cppmanip
{
namespace query
{

ast::LocalVariables findVariablesDeclaredByAndUsedAfterStmts(ast::StatementRange stmts, ast::StatementRange scope);

}
}
#endif // FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
