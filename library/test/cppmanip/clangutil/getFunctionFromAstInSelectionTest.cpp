#include <cppmanip/clangutil/getFunctionFromAstInSelection.hpp>
#include "ParsedFunction.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include <memory>


using namespace testing;

namespace cppmanip
{
namespace clangutil
{

struct getFunctionFromAstInSelectionTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    GetFunctionStatements getFunctionStatements;

    getFunctionFromAstInSelectionTest()
    {
        getFunctionStatements = [this](clang::FunctionDecl& f) { return getFunctionStatementsMocked(f); };
        ALLOWING_FCALL(getFunctionStatementsMocked(_)).WillRepeatedly(Return(ast::Statements()));
    }

    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
    }
    void assertFunctionAtOffsetContainsSelection(ast::SourceOffset offset, ast::SourceOffsetRange range)
    {
        ASSERT_EQ(offset, getFunctionFromAstInSelection(func->getASTContext(), range, getFunctionStatements)->getDefinitionOffset())
            << range;
    }
    void assertFailsForSelection(ast::SourceOffsetRange range)
    {
        ASSERT_THROW(getFunctionFromAstInSelection(func->getASTContext(), range, getFunctionStatements), boundary::ExtractMethodError)
            << range;
    }

    MOCK_METHOD1(getFunctionStatementsMocked, ast::Statements(clang::FunctionDecl& ));
};

TEST_F(getFunctionFromAstInSelectionTest, should_get_the_function_containing_given_selection)
{
    parse("void f()\n{\n  /*here*/\n}\n");
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(0, { 11, 19 }));
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(0, { 13, 13 }));
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(0, { 9, 10 }));
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(0, { 22, 23 }));
}

TEST_F(getFunctionFromAstInSelectionTest, should_fail_when_the_selection_is_not_overlapping_the_body)
{
    parse("void f()\n{\n /*...*/\n}\n");
    assertFailsForSelection({1, 9});
    assertFailsForSelection({21, 22});
}

TEST_F(getFunctionFromAstInSelectionTest, should_search_through_all_the_functions)
{
    parse("void f()\n{\n \n}\nvoid g()\n{\n /*here*/ \n}\nvoid h()\n{\n \n}\n");
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(15, { 27, 35 }));
}

TEST_F(getFunctionFromAstInSelectionTest, should_ignore_functions_without_bodies)
{
    parse("void f(); void g(); void h() { \n }"); // \n is needed because of clang bug
    ASSERT_NO_THROW(assertFunctionAtOffsetContainsSelection(20, { 30, 30 }));
}

TEST_F(getFunctionFromAstInSelectionTest, should_return_all_statements_of_the_function)
{
    parse("void f() {\nint x; int y = x + 2; }");
    ast::Statements stmts(7);
    EXPECT_FCALL(getFunctionStatementsMocked(Ref(*func->getDecl()))).WillRepeatedly(Return(stmts));
    ASSERT_TRUE(stmts == *getFunctionFromAstInSelection(func->getASTContext(), { 11, 11 }, getFunctionStatements)->getStatements());
}

}
}
