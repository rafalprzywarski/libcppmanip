#include <ClangToolArgsBuilder.hpp>
#include <gtest/gtest.h>

namespace cppmanip
{

struct ClangToolArgsBuilderTest : testing::Test
{
    ClangToolArgsBuilder builder;
    
    void expectArgs(std::vector<std::string> args)
    {
        ASSERT_EQ(args.size(), builder.getArgc());
        for (unsigned i = 0; i < args.size(); ++i)
            ASSERT_STREQ(args[i].c_str(), builder.getArgv()[i]) << "mismatched arg " << i;
    }
};

TEST_F(ClangToolArgsBuilderTest, should_pass_source_file_name_as_the_first_argument)
{
    builder.setSourceFilename("source");
    expectArgs({ "", "source", "--" });
}

}
