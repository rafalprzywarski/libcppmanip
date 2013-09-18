#include <OffsetConverter.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

TEST(OffsetConverterTest, should_return_offset_in_the_first_line)
{
    OffsetConverter coverter("xxxxxxxxxxxxxxx");
    SourceLocation loc = coverter.getLocationFromOffset(5);
    ASSERT_EQ(5, loc.col);
    ASSERT_EQ(0, loc.row);
}

TEST(OffsetConverterTest, should_return_offset_from_the_last_eol_as_col)
{
    OffsetConverter converter("x\nx\nxxxxx");
    ASSERT_EQ(2u, converter.getLocationFromOffset(6).col);
}

TEST(OffsetConverterTest, should_return_number_of_eols_till_offset_as_row)
{
    OffsetConverter converter("x\n\n\nxxxxx");
    EXPECT_EQ(0u, converter.getLocationFromOffset(0).row);
    EXPECT_EQ(3u, converter.getLocationFromOffset(4).row);
    EXPECT_EQ(3u, converter.getLocationFromOffset(5).row);
}

TEST(OffsetConverterTest, should_fail_if_offset_is_greater_or_equal_the_length_of_the_source)
{
    OffsetConverter converter(std::string(20, 'x'));
    ASSERT_NO_THROW(converter.getLocationFromOffset(19));
    ASSERT_THROW(converter.getLocationFromOffset(20), std::out_of_range);
    ASSERT_THROW(converter.getLocationFromOffset(21), std::out_of_range);
}