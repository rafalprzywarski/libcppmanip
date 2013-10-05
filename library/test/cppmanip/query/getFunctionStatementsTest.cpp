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
        ALLOWING_FCALL(getStmtRangeMocked(_, _)).WillRepeatedly(Return(ast::SourceLocationRange(ast::rowCol(0, 0), ast::rowCol(0, 0))));
    }

    void parse(std::string source)
    {
        func.reset(new test::ParsedFunction(source));
    }

    clang::Stmt& stmtNo(int index)
    {
        return **boost::next(func->getDecl()->getBody()->child_begin(), index);
    }

    MOCK_METHOD2(getStmtRangeMocked, ast::SourceLocationRange(clang::SourceManager&, clang::Stmt& ));
};

TEST_F(getFunctionStatementsTest, should_return_the_ranges_of_each_statements)
{
    parse("void f() {\n int x = 1; if (x == 1)\n; }");
    ast::SourceLocationRange LOC1{ { 1, 2 }, { 3, 4 } }, LOC2{ { 5, 6 }, { 7, 8 } };
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

}
}
