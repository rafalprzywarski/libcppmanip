#include <TextReplacementRecorder.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>

using namespace testing;

#define ALLOWING_CALL EXPECT_CALL

namespace cppmanip
{

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

TEST_F(TextReplacementRecorderTest, should_record_all_replacements)
{
    ALLOWING_CALL(*this, fromOffsetToSourceLocation(_)).Times(2 * 3).WillRepeatedly(Return(SourceLocation()));
    recorder.replaceWithTextInRange("a", 0, 0);
    recorder.replaceWithTextInRange("b", 0, 0);
    recorder.replaceWithTextInRange("c", 0, 0);
    SourceReplacements replacements = recorder.getReplacements();
    ASSERT_EQ(3u, replacements.size());
    ASSERT_EQ("a", replacements[0].text);
    ASSERT_EQ("b", replacements[1].text);
    ASSERT_EQ("c", replacements[2].text);
}

}
