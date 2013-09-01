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
