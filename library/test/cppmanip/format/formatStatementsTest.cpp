#include <cppmanip/format/formatStatements.hpp>
#include "../query/AstFactories.hpp"
#include <gtest/gtest.h>

namespace cppmanip
{
namespace format
{

TEST(formatStatementsTest, should_return_an_empty_string_for_no_statements)
{
    ASSERT_EQ("", formatStatements({}, 1));
}

TEST(formatStatementsTest, should_print_indented_statements)
{
    using query::test::stmtWithSourceCode;
    ast::Statements stmts = { stmtWithSourceCode("stmt1"), stmtWithSourceCode("stmt2") };
    ASSERT_EQ("     stmt1\n     stmt2\n", formatStatements(stmts, 5));
}

}
}
