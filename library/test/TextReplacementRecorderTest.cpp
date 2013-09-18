#include <TextReplacementRecorder.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>

using namespace testing;

struct TextReplacementRecorderTest : testing::Test
{
    TextReplacementRecorder recorder;
    SourceLocation FROM, TO;

    MOCK_METHOD1(fromOffsetToSourceLocation, SourceLocation(unsigned offset));

    TextReplacementRecorderTest()
        : recorder(std::bind(&TextReplacementRecorderTest::fromOffsetToSourceLocation, this, std::placeholders::_1))
    {
        FROM.col = 20;
        FROM.row = 30;
        TO.col = 40;
        TO.row = 50;
    }
};

TEST_F(TextReplacementRecorderTest, should_return_no_replacements_when_constructed)
{
    SourceReplacements replacements = recorder.getReplacements();
    ASSERT_TRUE(replacements.empty());
}

TEST_F(TextReplacementRecorderTest, should_record_a_text_replacement)
{
    EXPECT_CALL(*this, fromOffsetToSourceLocation(4)).WillOnce(Return(FROM));
    EXPECT_CALL(*this, fromOffsetToSourceLocation(7)).WillOnce(Return(TO));
    recorder.replaceWithTextInRange("text", 4, 7);
    SourceReplacements replacements = recorder.getReplacements();
    ASSERT_EQ(1u, replacements.size());
    ASSERT_EQ("text", replacements[0].text);
    ASSERT_EQ(FROM.row, replacements[0].from.row);
    ASSERT_EQ(FROM.col, replacements[0].from.col);
    ASSERT_EQ(TO.row, replacements[0].to.row);
    ASSERT_EQ(TO.col, replacements[0].to.col);
}
