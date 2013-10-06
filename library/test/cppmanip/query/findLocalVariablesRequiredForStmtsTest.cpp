#include <cppmanip/query/findLocalVariablesRequiredForStmts.hpp>
#include <cppmanip/ast/LocalVariable.hpp>
#include "../ParsedFunction.hpp"
#include "LocalVariablesAssert.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{
namespace test
{
using namespace cppmanip::test;

struct findLocalVariablesRequiredForStmtsTest : testing::Test
{
    ast::LocalVariablePtr var()
    {
        return std::make_shared<ast::LocalVariable>("", "");
    }

    ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars)
    {
        return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), ast::LocalVariables(), vars, "", "");
    }

    ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared)
    {
        return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), declared, ast::LocalVariables(), "", "");
    }
};

TEST_F(findLocalVariablesRequiredForStmtsTest, should_return_no_variables_if_none_are_used)
{
    ast::Statements stmts{ stmtWithUsedVars({}) };
    ASSERT_TRUE(findLocalVariablesRequiredForStmts(stmts).empty());
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_return_variables_in_given_range)
{
    auto var1 = var(), var2 = var(), var3 = var();
    ast::Statements stmts{ stmtWithUsedVars({var1}), stmtWithUsedVars({var2, var3}) };

    auto found = findLocalVariablesRequiredForStmts(stmts);
    expectEqUnordered(found, { var1, var2, var3 });
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_not_return_the_same_variable_twice)
{
    auto var1 = var();
    ast::Statements stmts{ stmtWithUsedVars({var1}), stmtWithUsedVars({var1}) };

    auto found = findLocalVariablesRequiredForStmts(stmts);
    ASSERT_EQ(1u, found.size());
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_not_return_variables_declared_by_statements)
{
    auto var1 = var(), var2 = var(), var3 = var(), var4 = var();
    ast::Statements stmts{ stmtWithDeclaredVars({var1, var2, var4}), stmtWithUsedVars({var1, var2, var3}) };

    auto found = findLocalVariablesRequiredForStmts(stmts);
    expectEqUnordered(found, { var3 });
}

TEST_F(findLocalVariablesRequiredForStmtsTest, DISABLED_should_return_variables_in_order_of_their_declaration)
{
    auto var1 = var(), var2 = var(), var3 = var();
    ast::Statements stmts{ stmtWithDeclaredVars({var1, var2}), stmtWithDeclaredVars({var3}) };

    auto found = findLocalVariablesRequiredForStmts(stmts);
    expectEqOrdered(found, { var1, var2, var3 });
}

}
}
}
