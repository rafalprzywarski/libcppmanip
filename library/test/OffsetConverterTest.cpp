#include <OffsetConverter.hpp>
#include <gtest/gtest.h>

TEST(OffsetConverterTest, should_return_offset_in_the_first_line)
{
    OffsetConverter coverter("xxxxxxxxxxxxxxx");
    SourceLocation loc = coverter.getLocationFromOffset(5);
    ASSERT_EQ(5, loc.col);
    ASSERT_EQ(0, loc.row);
}

TEST(OffsetConverterTest, should_return_offset_from_the_last_eol)
{
    OffsetConverter converter("x\nx\nxxxxx");
    auto loc = converter.getLocationFromOffset(6);
    ASSERT_EQ(2u, loc.col);
}
