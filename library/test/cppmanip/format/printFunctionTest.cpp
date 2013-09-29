#include <cppmanip/format/printFunction.hpp>
#include <gtest/gtest.h>

namespace cppmanip
{
namespace format
{

TEST(printFunctionCallTest, should_print_name_with_empty_parens_when_given_no_call_arguments)
{
    ASSERT_EQ("some_function()", printFunctionCall("some_function", std::vector<std::string>()));
}

TEST(printFunctionCallTest, should_print_the_argument_in_parens)
{
    ASSERT_EQ("f(arg)", printFunctionCall("f", { "arg" }));
}

TEST(printFunctionCallTest, should_print_the_arguments_separated_by_commas)
{
    ASSERT_EQ("f(arg1, arg2, arg3)", printFunctionCall("f", { "arg1", "arg2", "arg3" }));
}

TEST(printFunctionDefinitionTest, should_print_the_type_name_and_body_if_there_are_no_arguments)
{
    ASSERT_EQ("type name()\n{    body;\n}\n", printFunctionDefinition("type", "name", {}, "body;\n"));
}

}
}
