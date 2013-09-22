#include <cppmanip/DefaultClangFunctionLocator.hpp>
#include "ParsedFunction.hpp"
#include <cppmanip/SourceSelection.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace cppmanip
{

struct DefaultClangFunctionLocatorTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
    }
};

TEST_F(DefaultClangFunctionLocatorTest, should_get_the_function_containing_given_selection)
{
    parse("void f()\n{\n  /*here*/\n}\n");
    DefaultClangFunctionLocator locator(func->getASTContext());
    ASSERT_EQ("f", locator.getFunctionContainingSelection(OffsetRange(11, 19)).getNameAsString());
    ASSERT_EQ("f", locator.getFunctionContainingSelection(OffsetRange(13, 13)).getNameAsString());
    ASSERT_EQ("f", locator.getFunctionContainingSelection(OffsetRange(9, 10)).getNameAsString());
    ASSERT_EQ("f", locator.getFunctionContainingSelection(OffsetRange(22, 23)).getNameAsString());
}

TEST_F(DefaultClangFunctionLocatorTest, should_fail_when_the_selection_is_not_overlapping_the_body)
{
    parse("void f()\n{\n /*...*/\n}\n");
    DefaultClangFunctionLocator locator(func->getASTContext());
    ASSERT_THROW(locator.getFunctionContainingSelection(OffsetRange(1, 9)), std::runtime_error);
    ASSERT_THROW(locator.getFunctionContainingSelection(OffsetRange(21, 22)), std::runtime_error);
}

}