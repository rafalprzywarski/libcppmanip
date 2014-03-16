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

ast::LocalVariablePtr varWithNameAndNameWithType(const std::string& name, const std::string& nameWithType)
{
    return std::make_shared<ast::LocalVariable>(name, nameWithType, 0);
}

ast::StatementPtr stmt(std::vector<ast::Statements> children)
{
    ast::StatementGroups groups;
    for (auto const& c : children)
        groups.push_back(std::make_shared<ast::Statements>(c));
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::SourceOffsetRanges(), ast::LocalVariables(), ast::LocalVariables(), "", "", groups);
}

ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::SourceOffsetRanges(), ast::LocalVariables(), vars, "", "", ast::StatementGroups());
}

ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::SourceOffsetRanges(), declared, ast::LocalVariables(), "", "", ast::StatementGroups());
}

ast::StatementPtr stmtWithSourceCode(const std::string& sourceCode)
{
    return std::make_shared<ast::Statement>(ast::SourceOffsetRange(0, 0), ast::SourceOffsetRanges(), ast::LocalVariables(), ast::LocalVariables(), sourceCode, "", ast::StatementGroups());
}

ast::StatementPtr stmtWithRange(ast::SourceOffsetRange range)
{
    return std::make_shared<ast::Statement>(range, ast::SourceOffsetRanges(), ast::LocalVariables(), ast::LocalVariables(), "", "", ast::StatementGroups());
}

ast::StatementPtr stmtWithRangeAndSpecificRanges(ast::SourceOffsetRange range, ast::SourceOffsetRanges specific)
{
    return std::make_shared<ast::Statement>(range, specific, ast::LocalVariables(), ast::LocalVariables(), "", "", ast::StatementGroups());
}

ast::FunctionPtr functionWithStmts(ast::Statements stmts)
{
    return std::make_shared<ast::Function>(0, stmts);
}

}
}
}


