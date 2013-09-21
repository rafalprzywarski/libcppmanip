#include "SourceLocationConverter.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace cppmanip
{

TEST(SourceLocationConverterTest, should_return_column_at_row_0_as_offset)
{
    SourceLocationConverter converter("xxxxxx");
    SourceLocation loc;
    loc.col = 3;
    ASSERT_EQ(3u, converter.getOffsetFromLocation(loc));
}

TEST(SourceLocationConverterTest, should_sum_offset_from_previous_lines)
{
    SourceLocationConverter converter("xx\nxxx\nxx\nx");
    SourceLocation loc;
    loc.row = 2;
    loc.col = 1;
    ASSERT_EQ(8u, converter.getOffsetFromLocation(loc));
    loc.row = 3;
    ASSERT_EQ(11u, converter.getOffsetFromLocation(loc));
}

TEST(SourceLocationConverterTest, should_fail_of_row_is_greater_than_the_number_of_lines)
{
    SourceLocationConverter converter("\n\n");
    SourceLocation loc;
    loc.row = 2;
    ASSERT_NO_THROW(converter.getOffsetFromLocation(loc));
    loc.row = 3;
    ASSERT_THROW(converter.getOffsetFromLocation(loc), std::out_of_range);
}

}
