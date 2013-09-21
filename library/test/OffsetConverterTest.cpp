#include <cppmanip/OffsetConverter.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

namespace cppmanip
{

TEST(OffsetConverterTest, should_return_offset_in_the_first_line)
{
    OffsetConverter coverter("xxxxx\nx\nxxxxxxxxx");
    SourceLocation loc = coverter.getLocationFromOffset(5);
    ASSERT_EQ(5, loc.col);
    ASSERT_EQ(0, loc.row);
    ASSERT_EQ(4, coverter.getLocationFromOffset(4).col);
}

TEST(OffsetConverterTest, should_return_offset_from_the_last_eol_as_col)
{
    OffsetConverter converter("x\nx\nxxxxx\n");
    ASSERT_EQ(1u, converter.getLocationFromOffset(3).col);
    ASSERT_EQ(0u, converter.getLocationFromOffset(4).col);
    ASSERT_EQ(1u, converter.getLocationFromOffset(5).col);
}

TEST(OffsetConverterTest, should_return_number_of_eols_till_offset_as_row)
{
    OffsetConverter converter("x\n\n\nxxxxx\n");
    EXPECT_EQ(0u, converter.getLocationFromOffset(0).row);
    EXPECT_EQ(1u, converter.getLocationFromOffset(2).row);
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

}
