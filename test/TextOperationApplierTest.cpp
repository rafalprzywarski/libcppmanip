#include <gtest/gtest.h>
#include <TextOperationApplier.hpp>
#include <stdexcept>

struct TextOperationApplierTest : testing::Test
{
    TextOperationApplier applier;
    
    void shouldFailTryingToRemove(unsigned from, unsigned to)
    {
        ASSERT_THROW(applier.removeTextInRange(from, to), std::invalid_argument)
            << "should fail trying to remove text in range " << from << ".." << to;
    }
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
    shouldFailTryingToRemove(B - 2, B + 1);
    shouldFailTryingToRemove(B - 2, E);
    ASSERT_NO_THROW(applier.removeTextInRange(B - 2, B));
    shouldFailTryingToRemove(B, E + 1);
    shouldFailTryingToRemove(E - 1, E + 1);
    ASSERT_NO_THROW(applier.removeTextInRange(E, E + 1));
    shouldFailTryingToRemove(B, B + 2);
    shouldFailTryingToRemove(E - 2, E);
    shouldFailTryingToRemove(B - 2, E + 2);
    shouldFailTryingToRemove(B + 2, E - 2);
}

TEST_F(TextOperationApplierTest, should_fail_trying_to_remove_an_overlapping_empty_range)
{
    applier.removeTextInRange(2, 5);
    shouldFailTryingToRemove(3, 3);
}

TEST_F(TextOperationApplierTest, should_fail_trying_to_remove_an_invalid_range)
{
    shouldFailTryingToRemove(5, 4);
    ASSERT_NO_THROW(applier.removeTextInRange(3, 3));
}

TEST_F(TextOperationApplierTest, should_ignore_an_empty_range)
{
    applier.removeTextInRange(2, 2);
    ASSERT_NO_THROW(applier.removeTextInRange(2, 2));
    ASSERT_NO_THROW(applier.removeTextInRange(1, 3));
}

TEST_F(TextOperationApplierTest, should_perform_simultaneous_removals_and_insertions)
{
    applier.insertTextAt("burnt ", 5);
    applier.removeTextInRange(13, 18);
    applier.insertTextAt("black", 13);
    ASSERT_EQ("this burnt cake is black", applier.apply("this cake is tasty"));
}
