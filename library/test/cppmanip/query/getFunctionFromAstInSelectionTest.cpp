#include <cppmanip/query/getFunctionFromAstInSelection.hpp>
#include "../ParsedFunction.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include <memory>


using namespace testing;

namespace cppmanip
{
namespace query
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
    void assertFunctionContainsSelection(const std::string& name, ast::SourceOffset from, ast::SourceOffset to)
    {
        ASSERT_EQ(name, getFunctionFromAstInSelection(func->getASTContext(), {from, to}, getFunctionStatements)->getDecl().getNameAsString())
            << "[" << from << "; " << to << ")";
    }
    void assertFailsForSelection(ast::SourceOffset from, ast::SourceOffset to)
    {
        ASSERT_THROW(getFunctionFromAstInSelection(func->getASTContext(), {from, to}, getFunctionStatements), boundary::ExtractMethodError)
            << "[" << from << "; " << to << ")";
    }

    MOCK_METHOD1(getFunctionStatementsMocked, ast::Statements(clang::FunctionDecl& ));
};

TEST_F(getFunctionFromAstInSelectionTest, should_get_the_function_containing_given_selection)
{
    parse("void f()\n{\n  /*here*/\n}\n");
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", 11, 19));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", 13, 13));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", 9, 10));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", 22, 23));
}

TEST_F(getFunctionFromAstInSelectionTest, should_fail_when_the_selection_is_not_overlapping_the_body)
{
    parse("void f()\n{\n /*...*/\n}\n");
    assertFailsForSelection(1, 9);
    assertFailsForSelection(21, 22);
}

TEST_F(getFunctionFromAstInSelectionTest, should_search_through_all_the_functions)
{
    parse("void f()\n{\n \n}\nvoid g()\n{\n /*here*/ \n}\nvoid h()\n{\n \n}\n");
    ASSERT_NO_THROW(assertFunctionContainsSelection("g", 27, 35));
}

TEST_F(getFunctionFromAstInSelectionTest, should_ignore_functions_without_bodies)
{
    parse("void f(); void g(); void h() { \n }"); // \n is needed because of clang bug
    ASSERT_NO_THROW(assertFunctionContainsSelection("h", 30, 30));
}

TEST_F(getFunctionFromAstInSelectionTest, should_return_the_offset_of_the_function)
{
    parse("void f() { \n }\nvoid g() { \n }"); // \n is needed because of clang bug
    ASSERT_EQ(0, getFunctionFromAstInSelection(func->getASTContext(), { 10, 10 }, getFunctionStatements)->getDefinitionOffset());
    ASSERT_EQ(15, getFunctionFromAstInSelection(func->getASTContext(), { 25, 25 }, getFunctionStatements)->getDefinitionOffset());
}

TEST_F(getFunctionFromAstInSelectionTest, should_return_all_statements_of_the_function)
{
    parse("void f() {\nint x; int y = x + 2; }");
    ast::Statements stmts(7);
    EXPECT_FCALL(getFunctionStatementsMocked(Ref(*func->getDecl()))).WillRepeatedly(Return(stmts));
    ASSERT_TRUE(stmts == getFunctionFromAstInSelection(func->getASTContext(), { 11, 11 }, getFunctionStatements)->getStatements());
}

}
}
