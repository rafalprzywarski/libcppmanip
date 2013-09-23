#include <cppmanip/SourceLocationOperators.hpp>
#include <gtest/gtest.h>

namespace cppmanip
{

TEST(SourceLocationOperatorsTest, should_equal_iff_all_fields_equal)
{
    ASSERT_TRUE(rowCol(10, 15) == rowCol(10, 15));
    ASSERT_FALSE(rowCol(10, 15) == rowCol(7, 15));
    ASSERT_FALSE(rowCol(10, 15) == rowCol(10, 6));
    ASSERT_FALSE(rowCol(10, 15) == rowCol(2, 3));
}

TEST(SourceLocationOperatorsTest, should_order_by_row_then_by_column)
{
    ASSERT_TRUE(rowCol(5, 8) < rowCol(6, 8));
    ASSERT_FALSE(rowCol(6, 8) < rowCol(6, 8));
    ASSERT_FALSE(rowCol(7, 8) < rowCol(7, 8));

    ASSERT_TRUE(rowCol(1, 3) < rowCol(1, 4));
    ASSERT_FALSE(rowCol(1, 3) < rowCol(1, 3));
    ASSERT_FALSE(rowCol(1, 3) < rowCol(1, 2));

    ASSERT_TRUE(rowCol(1, 2) < rowCol(2, 1));
    ASSERT_FALSE(rowCol(2, 1) < rowCol(1, 2));
}

}
