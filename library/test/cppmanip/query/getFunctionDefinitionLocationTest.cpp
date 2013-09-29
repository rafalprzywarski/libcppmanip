#include <cppmanip/query/getFunctionDefinitionLocation.hpp>
#include "../ParsedFunction.hpp"
#include <gtest/gtest.h>

namespace cppmanip
{
namespace query
{

struct getFunctionDefinitionLocationTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> function;
};

TEST_F(getFunctionDefinitionLocationTest, should_return_the_location_of_the_definition)
{
    function.reset(new test::ParsedFunction("\n   void f() { }"));
    auto loc = getFunctionDefinitionLocation(*function->getDecl());
    ASSERT_EQ(4, function->getSourceManager().getFileOffset(loc));
}

}
}
