#ifndef CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#define CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#include <cppmanip/ast/LocalVariable.hpp>
#include <cppmanip/ast/Statement.hpp>

namespace cppmanip
{
namespace query
{

ast::LocalVariables findLocalVariablesRequiredForStmts(ast::StatementRange stmts);

}
}
#endif // CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
