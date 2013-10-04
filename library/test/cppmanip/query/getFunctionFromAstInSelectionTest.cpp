#include <cppmanip/query/getFunctionFromAstInSelection.hpp>
#include "../ParsedFunction.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <gmock/gmock.h>
#include <memory>

#define EXPECT_FCALL(call) EXPECT_CALL(*this, call)
#define ALLOWING_FCALL(call) EXPECT_CALL(*this, call)

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
    void assertFunctionContainsSelection(const std::string& name, ast::SourceLocation from, ast::SourceLocation to)
    {
        ASSERT_EQ(name, getFunctionFromAstInSelection(func->getASTContext(), LocationRange(from, to), getFunctionStatements)->getDecl().getNameAsString())
            << "[" << from << "; " << to << ")";
    }
    void assertFailsForSelection(ast::SourceLocation from, ast::SourceLocation to)
    {
        ASSERT_THROW(getFunctionFromAstInSelection(func->getASTContext(), LocationRange(from, to), getFunctionStatements), boundary::ExtractMethodError)
            << "[" << from << "; " << to << ")";
    }

    MOCK_METHOD1(getFunctionStatementsMocked, ast::Statements(clang::FunctionDecl& ));
};

TEST_F(getFunctionFromAstInSelectionTest, should_get_the_function_containing_given_selection)
{
    using cppmanip::ast::rowCol;
    parse("void f()\n{\n  /*here*/\n}\n");
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", rowCol(2, 0), rowCol(2, 8)));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", rowCol(2, 2), rowCol(2, 2)));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", rowCol(1, 0), rowCol(1, 1)));
    ASSERT_NO_THROW(assertFunctionContainsSelection("f", rowCol(3, 0), rowCol(3, 1)));
}

TEST_F(getFunctionFromAstInSelectionTest, should_fail_when_the_selection_is_not_overlapping_the_body)
{
    using cppmanip::ast::rowCol;
    parse("void f()\n{\n /*...*/\n}\n");
    assertFailsForSelection(rowCol(0, 1), rowCol(0, 9));
    assertFailsForSelection(rowCol(3, 1), rowCol(4, 0));
}

TEST_F(getFunctionFromAstInSelectionTest, should_search_through_all_the_functions)
{
    using cppmanip::ast::rowCol;
    parse("void f()\n{\n \n}\nvoid g()\n{\n /*here*/ \n}\nvoid h()\n{\n \n}\n");
    ASSERT_NO_THROW(assertFunctionContainsSelection("g", rowCol(6, 1), rowCol(6, 14)));
}

TEST_F(getFunctionFromAstInSelectionTest, should_ignore_functions_without_bodies)
{
    using cppmanip::ast::rowCol;
    parse("void f(); void g(); void h() { \n }"); // \n is needed because of clang bug
    ASSERT_NO_THROW(assertFunctionContainsSelection("h", rowCol(0, 30), rowCol(0, 30)));
}

TEST_F(getFunctionFromAstInSelectionTest, should_return_the_offset_of_the_function)
{
    using cppmanip::ast::rowCol;
    parse("void f() { \n }\nvoid g() { \n }"); // \n is needed because of clang bug
    ASSERT_EQ(0, getFunctionFromAstInSelection(func->getASTContext(), { rowCol(0, 10), rowCol(0, 10) }, getFunctionStatements)->getDefinitionOffset());
    ASSERT_EQ(15, getFunctionFromAstInSelection(func->getASTContext(), { rowCol(2, 10), rowCol(2, 10) }, getFunctionStatements)->getDefinitionOffset());
}

TEST_F(getFunctionFromAstInSelectionTest, should_return_all_statements_of_the_function)
{
    using cppmanip::ast::rowCol;
    parse("void f() {\nint x; int y = x + 2; }");
    ast::Statements stmts(7);
    EXPECT_FCALL(getFunctionStatementsMocked(Ref(*func->getDecl()))).WillRepeatedly(Return(stmts));
    ASSERT_TRUE(stmts == getFunctionFromAstInSelection(func->getASTContext(), { rowCol(1, 0), rowCol(1, 0) }, getFunctionStatements)->getStatements());
}

}
}
