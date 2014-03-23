#include <cppmanip/clangutil/getFunctionStatements.hpp>
#include "ParsedFunction.hpp"
#include <clang/AST/Stmt.h>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/find.hpp>

using namespace testing;

namespace cppmanip
{
namespace clangutil
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

    clang::Stmt *stmtNo(int index)
    {
        return *boost::next(func->getDecl()->getBody()->child_begin(), index);
    }

    clang::Stmt *stmtAt(std::vector<int> indices)
    {
        auto stmt = func->getDecl()->getBody();
        for (auto index : indices)
            stmt = *boost::next(stmt->child_begin(), index);
        return stmt;
    }

    void setRanges(std::vector<std::pair<clang::Stmt *, ast::SourceOffsetRange>> ranges)
    {
        for (auto r : ranges)
            EXPECT_FCALL(getStmtRangeMocked(_, Ref(*r.first))).WillRepeatedly(Return(r.second));
    }

    void expectVariableAtOffset(ast::LocalVariablePtr var, std::string name, std::string nameWithType, ast::SourceOffset offset)
    {
        EXPECT_EQ(name, var->getName());
        EXPECT_EQ(nameWithType, var->getNameWithType());
        EXPECT_EQ(offset, var->getDeclarationOffset());
    }

    void expectContainsRange(ast::SourceOffsetRanges ranges, ast::SourceOffsetRange expected)
    {
        ASSERT_TRUE(boost::find(ranges, expected) != ranges.end()) << expected << " not found";
    }

    MOCK_METHOD2(getStmtRangeMocked, ast::SourceOffsetRange(clang::SourceManager&, clang::Stmt& ));
};

TEST_F(getFunctionStatementsTest, should_return_the_ranges_of_each_statements)
{
    parse("void f() {\n int x = 1; if (x == 1)\n; }");
    ast::SourceOffsetRange range0{1, 2}, range1{3, 4};
    setRanges({{ stmtNo(0), range0 }, { stmtNo(1), range1 }});

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_EQ(2u, stmts.size());
    ASSERT_EQ(range0, stmts[0]->getRange());
    ASSERT_EQ(range1, stmts[1]->getRange());
}

TEST_F(getFunctionStatementsTest, should_return_no_statements_when_the_given_function_has_none)
{
    parse("void f() {\n }");
    ASSERT_TRUE(getFunctionStatements(*func->getDecl(), getStmtRange).empty());
}

TEST_F(getFunctionStatementsTest, should_return_local_variables_declared_by_a_statement_with_offsets)
{
    parse("void f() {\n int x = 1, y = 2, z = 1; }");
    auto vars = getFunctionStatements(*func->getDecl(), getStmtRange)[0]->getDeclaredVariables();
    ASSERT_EQ(3u, vars.size());
    expectVariableAtOffset(vars[0], "x", "int x", 16);
    expectVariableAtOffset(vars[1], "y", "int y", 23);
    expectVariableAtOffset(vars[2], "z", "int z", 30);
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

TEST_F(getFunctionStatementsTest, should_store_statments_source_code)
{
    parse("void f(int) {\n f(1 + 3); int a =\n 9; }");
    setRanges({ { stmtNo(0), {15, 24} }, { stmtNo(1), {25, 36} } });

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    EXPECT_EQ("f(1 + 3);", stmts[0]->getSourceCode());
    ASSERT_EQ("int a =\n 9;", stmts[1]->getSourceCode());
}

TEST_F(getFunctionStatementsTest, should_store_source_code_between_statements)
{
    parse("void f() {\n f();/* text */f();  /* whitespace */  f( );// not stored\n }");
    setRanges({ { stmtNo(0), {12, 16} }, { stmtNo(1), {26, 30} }, { stmtNo(2), {50, 55} } });

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    EXPECT_EQ("/* text */", stmts[0]->getSourceCodeAfter());
    EXPECT_EQ("  /* whitespace */  ", stmts[1]->getSourceCodeAfter());
    EXPECT_EQ("", stmts[2]->getSourceCodeAfter());
}

TEST_F(getFunctionStatementsTest, should_build_ast_for_try_statement)
{
    parse("void f() {\n try { int a; int b; int c; } catch (int) { } catch (...) { } }");

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_EQ(3u, stmts[0]->getChildGroups().size());
}

TEST_F(getFunctionStatementsTest, should_build_ast_for_try_block_children)
{
    parse("void f() {\n try { int a; int b; int c; } catch (...) { } }");

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_EQ(3u, stmts[0]->getChildGroups().at(0)->size());
    ASSERT_EQ(1u, stmts[0]->getChildGroups().at(0)->at(1)->getDeclaredVariables().size());
}

TEST_F(getFunctionStatementsTest, should_build_ast_for_catch_block_children)
{
    parse("void f() {\n try { } catch (int) { } catch (...) { int a; int b; int c; } }");

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);
    ASSERT_EQ(3u, stmts[0]->getChildGroups().at(2)->size());
    ASSERT_EQ(1u, stmts[0]->getChildGroups().at(2)->at(1)->getDeclaredVariables().size());
}

TEST_F(getFunctionStatementsTest, should_return_try_as_statement_specific_range)
{
    parse("void f() {\n /*from*/try {/*to*/ } catch (...) { } }");

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);

    expectContainsRange(stmts[0]->getSpecificRanges(), { 20, 25 });
}

TEST_F(getFunctionStatementsTest, should_return_catch_and_its_opening_brace_as_statement_specific_range)
{
    parse("void f() {\n try { } /*from*/catch (...) {/*to*/ } }");

    auto stmts = getFunctionStatements(*func->getDecl(), getStmtRange);

    expectContainsRange(stmts[0]->getSpecificRanges(), { 28, 41 });
}

}
}
