#ifndef CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
#define CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
#include <cppmanip/ast/Statement.hpp>
#include <cppmanip/ast/Function.hpp>

namespace cppmanip
{
namespace query
{
namespace test
{

ast::LocalVariablePtr var();
ast::LocalVariablePtr varDeclaredAtOffset(ast::SourceOffset offset);
ast::LocalVariablePtr varWithNameAndNameWithType(const std::string& name, const std::string& nameWithType);
ast::StatementPtr stmt(std::vector<ast::Statements> children = {});
ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars);
ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared);
ast::StatementPtr stmtWithSourceCode(const std::string& sourceCode);
ast::FunctionPtr functionWithStmts(ast::Statements stmts);

}
}
}
#endif // CPPMANIP_A974D0E5CE574C3D924423C93A8C1199_STATEMENTFACTORIES_HPP
