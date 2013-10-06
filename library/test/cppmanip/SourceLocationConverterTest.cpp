#include <cppmanip/text/SourceLocationConverter.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

namespace cppmanip
{
namespace text
{

TEST(SourceLocationConverterTest, should_return_column_at_row_0_as_offset)
{
    SourceLocationConverter converter("xxxxxx");
    ASSERT_EQ(3u, converter.getOffsetFromLocation(boundary::rowCol(0, 3)));
}

TEST(SourceLocationConverterTest, should_sum_offset_from_previous_lines)
{
    SourceLocationConverter converter("xx\nxxx\nxx\nx");
    ASSERT_EQ(8u, converter.getOffsetFromLocation(boundary::rowCol(2, 1)));
    ASSERT_EQ(11u, converter.getOffsetFromLocation(boundary::rowCol(3, 1)));
}

TEST(SourceLocationConverterTest, should_fail_of_row_is_greater_than_the_number_of_lines)
{
    SourceLocationConverter converter("\n\n");
    ASSERT_NO_THROW(converter.getOffsetFromLocation(boundary::rowCol(2, 0)));
    ASSERT_THROW(converter.getOffsetFromLocation(boundary::rowCol(3, 0)), std::out_of_range);
}

}
}
