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
    parse("void f() { /*here*/ }");
    DefaultClangFunctionLocator locator(func->getASTContext());
    ASSERT_EQ("f", locator.getFunctionContainingSelection(OffsetRange(11, 19)).getNameAsString());
}

}