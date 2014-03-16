#include <cppmanip/query/isStatementSelected.hpp>
#include "AstFactories.hpp"
#include <gtest/gtest.h>

namespace cppmanip
{
namespace query
{
namespace test
{

TEST(isStatementSelectedTest, should_return_IMPLICITLY_when_a_statement_range_overlaps_a_selection)
{
    ASSERT_TRUE(isStatementSelected(stmtWithRange({1, 3}), { 0, 2 }) == StatementSelected::IMPLICITLY);
}

TEST(isStatementSelectedTest, should_return_NO_when_a_statement_range_does_not_overlap_a_selection)
{
    ASSERT_TRUE(isStatementSelected(stmtWithRange({0, 1}), { 3, 4 }) == StatementSelected::NO);
}

TEST(isStatementSelectedTest, should_return_EXPLICITLY_when_a_statement_specific_ranges_overlap_a_selection)
{
    auto stmt = stmtWithRangeAndSpecificRanges({0, 6}, {{0, 2}, {4, 6}});
    ASSERT_TRUE(isStatementSelected(stmt, {0, 1}) == StatementSelected::EXPLICITLY);
    ASSERT_TRUE(isStatementSelected(stmt, {4, 5}) == StatementSelected::EXPLICITLY);
    ASSERT_TRUE(isStatementSelected(stmt, {2, 3}) == StatementSelected::IMPLICITLY);
    ASSERT_TRUE(isStatementSelected(stmt, {6, 7}) == StatementSelected::NO);
}

}
}
}
