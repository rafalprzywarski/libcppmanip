#include <gtest/gtest.h>
#include <OffsetRange.hpp>

struct OffsetRangeTest : testing::Test
{
    void expectNoOverlapping(OffsetRange left, OffsetRange right, const std::string& desc)
    {
        ASSERT_FALSE(left.overlapsWith(right)) << desc;
        ASSERT_FALSE(right.overlapsWith(left)) << desc;
    }
};

TEST_F(OffsetRangeTest, should_be_empty_by_default)
{
    ASSERT_TRUE(OffsetRange().empty());
}

TEST_F(OffsetRangeTest, should_have_zero_length_by_default)
{
    ASSERT_EQ(0, OffsetRange().length());
}

TEST_F(OffsetRangeTest, two_degenerate_ranges_do_not_overlap)
{
    expectNoOverlapping({2, 2}, {2, 2}, "same ranges");
    expectNoOverlapping({1, 1}, {2, 2}, "different ranges");
}