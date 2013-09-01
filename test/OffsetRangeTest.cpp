#include <gtest/gtest.h>
#include <OffsetRange.hpp>

TEST(OffsetRangeTest, should_be_empty_by_default)
{
    ASSERT_TRUE(OffsetRange().empty());
}

TEST(OffsetRangeTest, should_have_zero_length_by_default)
{
    ASSERT_EQ(0, OffsetRange().length());
}