#include <gtest/gtest.h>
#include <TextOperationApplier.hpp>

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

TEST_F(TextOperationApplierTest, should_perform_multiple_insertions)
{
    applier.insertTextAt("green", 0);
    applier.insertTextAt("blue", 5);
    ASSERT_EQ("green and blue are inserted", applier.apply(" and  are inserted"));
}

TEST_F(TextOperationApplierTest, should_perform_insertions_in_given_order)
{
    applier.insertTextAt("a", 0);
    applier.insertTextAt("b", 0);
    applier.insertTextAt("c", 0);
    ASSERT_EQ("abc", applier.apply(""));
}
