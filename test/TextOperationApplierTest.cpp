#include <gtest/gtest.h>
#include <TextOperationApplier.hpp>
#include <stdexcept>

struct TextOperationApplierTest : testing::Test
{
    TextOperationApplier applier;
};

TEST_F(TextOperationApplierTest, should_not_modify_given_text_when_no_operations_where_added)
{
    ASSERT_EQ("something", applier.apply("something"));
}

TEST_F(TextOperationApplierTest, should_insert_text_at_given_offset)
{
    applier.insertTextAt("is", 5);
    ASSERT_EQ("This is a text", applier.apply("This  a text"));
}

TEST_F(TextOperationApplierTest, should_perform_multiple_insertions_at_offsets_given_in_any_order)
{
    applier.insertTextAt("green", 2);
    applier.insertTextAt("blue", 7);
    applier.insertTextAt("red", 0);
    ASSERT_EQ("red, green and blue are inserted", applier.apply(",  and  are inserted"));
}

TEST_F(TextOperationApplierTest, should_perform_insertions_in_given_order)
{
    applier.insertTextAt("a", 0);
    applier.insertTextAt("b", 0);
    applier.insertTextAt("c", 0);
    ASSERT_EQ("abc", applier.apply(""));
}

TEST_F(TextOperationApplierTest, should_remove_text_at_given_range)
{
    applier.removeTextInRange(3, 11);
    ASSERT_EQ("it works", applier.apply("it bla bla works"));
}

TEST_F(TextOperationApplierTest, should_perform_multiple_removals_at_ranges_given_in_any_order)
{
    applier.removeTextInRange(0, 4);
    applier.removeTextInRange(7, 10);
    applier.removeTextInRange(4, 7);
    ASSERT_EQ("", applier.apply("just empty"));
}

TEST_F(TextOperationApplierTest, should_fail_trying_to_remove_overlapping_ranges)
{
    unsigned B = 3, E = 9;
    applier.removeTextInRange(3, 9);
    ASSERT_THROW(applier.removeTextInRange(B - 2, B + 1), std::invalid_argument);
    ASSERT_THROW(applier.removeTextInRange(B - 2, E), std::invalid_argument);
    ASSERT_NO_THROW(applier.removeTextInRange(B - 2, B));
    ASSERT_THROW(applier.removeTextInRange(B, E + 1), std::invalid_argument);
    ASSERT_THROW(applier.removeTextInRange(E - 1, E + 1), std::invalid_argument);
    ASSERT_NO_THROW(applier.removeTextInRange(E, E + 1));
    ASSERT_THROW(applier.removeTextInRange(B, B + 2), std::invalid_argument);
    ASSERT_THROW(applier.removeTextInRange(E - 2, E), std::invalid_argument);
    ASSERT_THROW(applier.removeTextInRange(B - 2, E + 2), std::invalid_argument);
    ASSERT_THROW(applier.removeTextInRange(B + 2, E - 2), std::invalid_argument);
}

TEST_F(TextOperationApplierTest, should_fail_trying_to_remove_an_invalid_range)
{
    ASSERT_THROW(applier.removeTextInRange(5, 4), std::invalid_argument);
    ASSERT_NO_THROW(applier.removeTextInRange(3, 3));
}
