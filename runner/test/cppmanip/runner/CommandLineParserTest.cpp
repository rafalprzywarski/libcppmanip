#include <gtest/gtest.h>
#include <cppmanip/runner/CommandLineParser.hpp>

namespace cppmanip
{
namespace runner
{

struct CommandLineParserTest : testing::Test
{
    CommandLineParser parser;
};

TEST_F(CommandLineParserTest, should_parse_extract_function_request)
{
    std::vector<const char *> args{ "", "file", "method", "20", "30", "40", "50" };
    auto req = parser.parseExtractFunction(args.size(), &args[0]);
    ASSERT_EQ("file", req.sourceFilename);
    ASSERT_EQ(1u, req.locations.size());
    ASSERT_EQ("method", req.locations[0].extractedMethodName);
    ASSERT_EQ(20, req.locations[0].sourceSelection.from.row);
    ASSERT_EQ(30, req.locations[0].sourceSelection.from.col);
    ASSERT_EQ(40, req.locations[0].sourceSelection.to.row);
    ASSERT_EQ(50, req.locations[0].sourceSelection.to.col);
}

TEST_F(CommandLineParserTest, should_parse_all_subsequent_request)
{
    std::vector<const char *> args{ "", "file", "method1", "11", "12", "13", "14", "method2", "21", "22", "23", "24" };
    auto req = parser.parseExtractFunction(args.size(), &args[0]);
    ASSERT_EQ(2u, req.locations.size());
    ASSERT_EQ("method1", req.locations[0].extractedMethodName);
    ASSERT_EQ(11, req.locations[0].sourceSelection.from.row);
    ASSERT_EQ("method2", req.locations[1].extractedMethodName);
    ASSERT_EQ(21, req.locations[1].sourceSelection.from.row);
}

}
}
