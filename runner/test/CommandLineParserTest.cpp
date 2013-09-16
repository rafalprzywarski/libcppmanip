#include <gtest/gtest.h>
#include "../src/CommandLineParser.hpp"

struct CommandLineParserTest : testing::Test
{
    CommandLineParser parser;
};

TEST_F(CommandLineParserTest, should_parse_extract_method_request)
{
    std::vector<const char *> args{ "", "file", "method", "20", "40" };
    auto req = parser.parseExtractMethod(args.size(), &args[0]);
    ASSERT_EQ("file", req.sourceFilename);
    ASSERT_EQ(1u, req.locations.size());
    ASSERT_EQ("method", req.locations[0].extractedMethodName);
    ASSERT_EQ(20, req.locations[0].sourceSelection.from);
    ASSERT_EQ(40, req.locations[0].sourceSelection.to);
}

TEST_F(CommandLineParserTest, should_parse_all_subsequent_request)
{
    std::vector<const char *> args{ "", "file", "method1", "11", "12", "method2", "21", "22" };
    auto req = parser.parseExtractMethod(args.size(), &args[0]);
    ASSERT_EQ(2u, req.locations.size());
    ASSERT_EQ("method1", req.locations[0].extractedMethodName);
    ASSERT_EQ(11, req.locations[0].sourceSelection.from);
    ASSERT_EQ(12, req.locations[0].sourceSelection.to);
    ASSERT_EQ("method2", req.locations[1].extractedMethodName);
    ASSERT_EQ(21, req.locations[1].sourceSelection.from);
    ASSERT_EQ(22, req.locations[1].sourceSelection.to);
}

