#include <gtest/gtest.h>
#include <cppmanip/text/OffsetRange.hpp>
#include <stdexcept>

namespace cppmanip
{

struct OffsetRangeTest : testing::Test
{
    void expectNoOverlapping(OffsetRange left, OffsetRange right, const std::string& desc)
    {
        ASSERT_FALSE(left.overlapsWith(right)) << desc;
        ASSERT_FALSE(right.overlapsWith(left)) << desc;
    }
    void expectOverlapping(OffsetRange left, OffsetRange right, const std::string& desc)
    {
        ASSERT_TRUE(left.overlapsWith(right)) << desc;
        ASSERT_TRUE(right.overlapsWith(left)) << desc;
    }
};

TEST_F(OffsetRangeTest, should_be_degenerate_by_default)
{
    ASSERT_TRUE(OffsetRange().degenerate());
}

TEST_F(OffsetRangeTest, should_detect_degenerate_ranges)
{
    ASSERT_TRUE(OffsetRange(2, 2).degenerate());
    ASSERT_FALSE(OffsetRange(3, 5).degenerate());
}

TEST_F(OffsetRangeTest, should_fail_when_trying_to_construct_an_invalid_range)
{
    ASSERT_THROW(OffsetRange(5, 2), std::invalid_argument);
}

TEST_F(OffsetRangeTest, two_degenerate_ranges_do_not_overlap)
{
    expectNoOverlapping({2, 2}, {2, 2}, "same ranges");
    expectNoOverlapping({1, 1}, {2, 2}, "different ranges");
}

TEST_F(OffsetRangeTest, denerate_ranges_overlap_when_not_on_bounds)
{
    expectNoOverlapping({1, 3}, {1, 1}, "beginnig");
    expectOverlapping({1, 3}, {2, 2}, "middle");
    expectNoOverlapping({1, 3}, {3, 3}, "ending");
}

TEST_F(OffsetRangeTest, disjoint_ranges_do_not_overlap)
{
    expectNoOverlapping({1, 3}, {3, 5}, "near");
    expectNoOverlapping({1, 3}, {4, 6}, "far");
}

TEST_F(OffsetRangeTest, should_detect_overlapping_ranges)
{
    expectOverlapping({1, 4}, {3, 5}, "intersecting");
    expectOverlapping({1, 10}, {3, 7}, "containing");
    expectOverlapping({1, 4}, {1, 3}, "common beginning");
    expectOverlapping({1, 5}, {2, 5}, "common ending");
}

TEST_F(OffsetRangeTest, ranges_are_equal_iff_they_have_the_same_bounds)
{
    ASSERT_TRUE(OffsetRange(2, 5) == OffsetRange(2, 5));
    ASSERT_FALSE(OffsetRange(2, 3) == OffsetRange(2, 5));
    ASSERT_FALSE(OffsetRange(1, 5) == OffsetRange(2, 5));
}

}
