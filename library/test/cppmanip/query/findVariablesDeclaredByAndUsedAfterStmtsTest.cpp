#include <cppmanip/query/findVariablesDeclaredByAndUsedAfterStmts.hpp>
#include "LocalVariablesAssert.hpp"
#include "AstFactories.hpp"
#include <gtest/gtest.h>
#include <memory>

namespace cppmanip
{
namespace query
{
namespace test
{

struct findVariablesDeclaredByAndUsedAfterStmtsTest : testing::Test
{
};

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_no_variables_if_none_are_declared)
{
    ast::Statements stmts{ stmt(), stmt() };

    ASSERT_TRUE(findVariablesDeclaredByAndUsedAfterStmts(stmts, stmts).empty());
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_variables_used_after_given_statements)
{
    auto var1 = var(), var2 = var(), var3 = var();
    ast::Statements stmts{ stmtWithDeclaredVars({var1}), stmtWithDeclaredVars({var2, var3}), stmtWithUsedVars({var2}), stmtWithUsedVars({var1, var2}) };

    auto found = findVariablesDeclaredByAndUsedAfterStmts({stmts.begin(), stmts.begin() + 2}, stmts);
    expectEqUnordered(found, { var1, var2 });
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_only_return_variables_declared_by_given_statements)
{
    auto var1 = var(), var2 = var(), var3 = var(), var4 = var();
    ast::Statements stmts{
        stmtWithDeclaredVars({var1, var2}), stmtWithDeclaredVars({var3}), stmtWithDeclaredVars({var4}),
        stmtWithUsedVars({var1, var2}), stmtWithUsedVars({var3, var4}) };

    auto found = findVariablesDeclaredByAndUsedAfterStmts({stmts.begin() + 1, stmts.begin() + 3}, stmts);
    expectEqUnordered(found, { var3, var4 });
}

}
}
}
