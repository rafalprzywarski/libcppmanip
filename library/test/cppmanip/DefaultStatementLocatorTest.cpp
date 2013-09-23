#include <cppmanip/DefaultStatementLocator.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>

namespace cppmanip
{

struct DefaultStatementLocatorTest : testing::Test
{
};

TEST_F(DefaultStatementLocatorTest, should_return_an_empty_range_for_an_empty_function)
{
    test::ParsedFunction f("void f() {\n \n}\n");
    DefaultStatementLocator locator(LocationRange(rowCol(1, 1), rowCol(1, 1)));
    auto stmts = locator.findStatementsInFunction(*f.getFunction());
    ASSERT_TRUE(stmts.empty());
}

}
