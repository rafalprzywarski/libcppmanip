#ifndef CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
#define CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
#include <cppmanip/ast/Statement.hpp>

namespace cppmanip
{
namespace query
{
namespace test
{

ast::LocalVariablePtr var();
ast::StatementPtr stmt();
ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars);
ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared);

}
}
}
#endif // CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
