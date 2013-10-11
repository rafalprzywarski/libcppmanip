#include "AstFactories.hpp"

namespace cppmanip
{
namespace query
{
namespace test
{

ast::LocalVariablePtr var()
{
    return varDeclaredAtOffset(0);
}

ast::LocalVariablePtr varDeclaredAtOffset(ast::SourceOffset offset)
{
    return std::make_shared<ast::LocalVariable>("", "", offset);
}

ast::StatementPtr stmt(ast::Statements children)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::LocalVariables(), ast::LocalVariables(), "", "", children);
}

ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::LocalVariables(), vars, "", "", ast::Statements());
}

ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), declared, ast::LocalVariables(), "", "", ast::Statements());
}

}
}
}


