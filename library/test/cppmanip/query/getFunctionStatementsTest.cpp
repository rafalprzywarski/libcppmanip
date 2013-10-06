#include <cppmanip/query/getFunctionStatements.hpp>
#include "../ParsedFunction.hpp"
#include <clang/AST/Stmt.h>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include <boost/next_prior.hpp>

using namespace testing;

namespace cppmanip
{
namespace query
{

struct getFunctionStatementsTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    GetStatementRange getStmtRange;

    getFunctionStatementsTest()
    {
        getStmtRange = [this](clang::SourceManager& sm, clang::Stmt& s) { return getStmtRangeMocked(sm, s); };
        ALLOWING_FCALL(getStmtRangeMocked(_, _)).WillRepeatedly(Return(ast::SourceOffsetRange(0, 0)));
    }

    void parse(std::string source)
    {
        func.reset(new test::ParsedFunction(source));
    }

    clang::Stmt& stmtNo(int index)
    {
        return **boost::next(func->getDecl()->getBody()->child_begin(), index);
    }

    MOCK_METHOD2(getStmtRangeMocked, ast::SourceOffsetRange(clang::SourceManager&, clang::Stmt& ));
};

TEST_F(getFunctionStatementsTest, should_return_the_ranges_of_each_statements)
{
    parse("void f() {\n int x = 1; if (x == 1)\n; }");
    ast::SourceOffsetRange LOC1{1, 2}, LOC2{3, 4};
    EXPECT_FCALL(getStmtRangeMocked(_, Ref(stmtNo(0)))).WillOnce(Return(LOC1));
    EXPECT_FCALL(getStmtRangeMocked(_, Ref(stmtNo(1)))).WillOnce(Return(LOC2));
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);

    ASSERT_EQ(2u, stmts.size());
    ASSERT_EQ(LOC1, stmts[0]->getRange());
    ASSERT_EQ(LOC2, stmts[1]->getRange());
}

TEST_F(getFunctionStatementsTest, should_return_no_statements_when_the_given_function_has_none)
{
    parse("void f() {\n }");
    ASSERT_TRUE(getFunctionStatements(*func->getDecl(), getStmtRange).empty());
}

TEST_F(getFunctionStatementsTest, should_return_local_variables_declared_by_a_statement)
{
    parse("void f() {\n int x = 1, y = 2, z = 1; }");
    auto vars = getFunctionStatements(*func->getDecl(), getStmtRange)[0]->getDeclaredVariables();
    ASSERT_EQ(3u, vars.size());
    EXPECT_EQ("x", vars[0]->getName());
    EXPECT_EQ("int x", vars[0]->getNameWithType());
    EXPECT_EQ("y", vars[1]->getName());
    EXPECT_EQ("int y", vars[1]->getNameWithType());
    EXPECT_EQ("z", vars[2]->getName());
    EXPECT_EQ("int z", vars[2]->getNameWithType());
}

TEST_F(getFunctionStatementsTest, should_return_no_declared_local_variables_when_a_statement_declares_none)
{
    parse("void f() {\n f(); }");
    ASSERT_TRUE(getFunctionStatements(*func->getDecl(), getStmtRange)[0]->getDeclaredVariables().empty());
}

TEST_F(getFunctionStatementsTest, should_return_local_variables_used_by_a_statement)
{
    parse("void f(int) {\n int x = 2; int y = 3; int z = x; f(z + y); }");
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_TRUE(stmts[0]->getDeclaredVariables() == stmts[2]->getUsedLocalVariables());
    ASSERT_EQ(2u, stmts[3]->getUsedLocalVariables().size());
}

TEST_F(getFunctionStatementsTest, should_not_return_the_same_used_variable_more_than_once)
{
    parse("void f() {\n int x = 2; int z = x + x * x; }");
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_EQ(1u, stmts[1]->getUsedLocalVariables().size());
}

TEST_F(getFunctionStatementsTest, should_not_report_global_variables_as_used)
{
    parse("int global; void f() {\n int x = global; }");
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_TRUE(stmts[0]->getUsedLocalVariables().empty());
}

TEST_F(getFunctionStatementsTest, should_not_report_variables_not_visible_outside_of_statements)
{
    parse("int global; void f() {\n for(int x = 1; x < 2; ++x) { } }");
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_TRUE(stmts[0]->getUsedLocalVariables().empty());
}

TEST_F(getFunctionStatementsTest, should_temporarily_store_clang_statements)
{
    parse("void f() {\n int x; }");
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_TRUE(stmts[0]->getStmt() == &stmtNo(0));
}

TEST_F(getFunctionStatementsTest, should_store_statments_source_code)
{
    parse("void f(int) {\n f(1 + 3); int a =\n 9; }");
    ast::SourceOffsetRange f{15, 24}, a{25, 36};
    EXPECT_FCALL(getStmtRangeMocked(_, Ref(stmtNo(0)))).WillOnce(Return(f));
    EXPECT_FCALL(getStmtRangeMocked(_, Ref(stmtNo(1)))).WillOnce(Return(a));
    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    EXPECT_EQ("f(1 + 3);", stmts[0]->getSourceCode());
    ASSERT_EQ("int a =\n 9;", stmts[1]->getSourceCode());
}

}
}
