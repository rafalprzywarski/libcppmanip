#include "AstFactories.hpp"

namespace cppmanip
{
namespace query
{
namespace test
{

ast::LocalVariablePtr var()
{
    return std::make_shared<ast::LocalVariable>("", "");
}

ast::StatementPtr stmt()
{
    return stmtWithUsedVars({});
}

ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars)
{
    return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), ast::LocalVariables(), vars, "", "");
}

ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared)
{
    return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), declared, ast::LocalVariables(), "", "");
}

}
}
}


