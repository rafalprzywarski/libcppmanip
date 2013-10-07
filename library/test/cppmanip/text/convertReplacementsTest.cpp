#include <cppmanip/text/convertReplacements.hpp>
#include <cppmanip/text/OperationRecorder.hpp>
#include <cppmanip/ast/SourceOffset.hpp>
#include <cppmanip/boundary/SourceLocation.hpp>
#include <gtest/gtest.h>

namespace cppmanip
{

TEST(convertReplacementTest, should_convert_text_replacements_to_boundary_replacements)
{
    text::Replacements<ast::SourceOffset> replacements = { { 2, 3, "x" }, { 5, 7, "y" } };
    auto converted = convertReplacements(replacements, [](ast::SourceOffset offset) { return boundary::rowCol(offset * 11, offset * 111); });
    EXPECT_EQ(2u, converted.size());
    EXPECT_EQ(22u, converted[0].from.row);
    EXPECT_EQ(222u, converted[0].from.col);
    EXPECT_EQ(33u, converted[0].to.row);
    EXPECT_EQ(333u, converted[0].to.col);
    EXPECT_EQ("x", converted[0].text);
    EXPECT_EQ(55u, converted[1].from.row);
    EXPECT_EQ(77u, converted[1].to.row);
    EXPECT_EQ("y", converted[1].text);
}

}