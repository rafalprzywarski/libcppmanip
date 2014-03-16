#include <cppmanip/query/isStatementSelected.hpp>
#include "AstFactories.hpp"
#include <gtest/gtest.h>

namespace cppmanip
{
namespace query
{
namespace test
{

TEST(isStatementSelectedTest, should_return_IMPLICITLY_when_a_statement_range_overlaps_with_a_selection)
{
    ASSERT_TRUE(isStatementSelected(stmtWithRange({1, 3}), { 0, 2 }) == StatementSelected::IMPLICITLY);
}

TEST(isStatementSelectedTest, should_return_NO_when_a_statement_range_does_not_overlap_with_a_selection)
{
    ASSERT_TRUE(isStatementSelected(stmtWithRange({0, 1}), { 3, 4 }) == StatementSelected::NO);
}

}
}
}
