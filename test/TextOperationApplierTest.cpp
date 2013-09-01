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