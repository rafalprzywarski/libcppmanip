#include <cppmanip/query/getFunctionFromAstInSelection.hpp>
#include "../ParsedFunction.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace cppmanip
{
namespace query
{

struct getFunctionFromAstInSelectionTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
    }
    void assertFunctionContainsSelection(const std::string& name, ast::SourceLocation from, ast::SourceLocation to)
    {
        ASSERT_EQ(name, getFunctionFromAstInSelection(func->getASTContext(), LocationRange(from, to)).getDecl().getNameAsString())
            << "[" << from << "; " << to << ")";
    }
    void assertFailsForSelection(ast::SourceLocation from, ast::SourceLocation to)
    {
        ASSERT_THROW(getFunctionFromAstInSelection(func->getASTContext(), LocationRange(from, to)), boundary::ExtractMethodError)
            << "[" << from << "; " << to << ")";
    }
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

}
}
