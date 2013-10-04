#include <cppmanip/query/getFunctionStatements.hpp>
#include "../ParsedFunction.hpp"
#include <clang/AST/Stmt.h>
#include <gmock/gmock.h>

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
    }

    void parse(std::string source)
    {
        func.reset(new test::ParsedFunction(source));
    }

    MOCK_METHOD2(getStmtRangeMocked, ast::SourceLocationRange(clang::SourceManager&, clang::Stmt& ));
};

TEST_F(getFunctionStatementsTest, should_return_the_ranges_of_each_statements)
{
    parse("void f() {\n int x = 1; if (x == 1)\n; }");
    ast::SourceLocationRange LOC1{ { 1, 2 }, { 3, 4 } }, LOC2{ { 5, 6 }, { 7, 8 } };
    EXPECT_CALL(*this, getStmtRangeMocked(_, Ref(**func->getDecl()->getBody()->child_begin()))).WillOnce(Return(LOC1));
    EXPECT_CALL(*this, getStmtRangeMocked(_, Ref(**++func->getDecl()->getBody()->child_begin()))).WillOnce(Return(LOC2));
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

}
}
