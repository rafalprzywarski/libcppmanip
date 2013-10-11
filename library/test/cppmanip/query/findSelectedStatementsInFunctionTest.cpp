#include <cppmanip/query/findSelectedStatementsInFunction.hpp>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include "AstFactories.hpp"

using namespace testing;

namespace cppmanip
{
namespace query
{
namespace test
{

struct findStatementsInFunctionOverlappingSelectionTest : testing::Test
{
    MOCK_METHOD1(isSelected, bool(ast::StatementPtr));

    findStatementsInFunctionOverlappingSelectionTest()
    {
        EXPECT_FCALL(isSelected(_)).WillRepeatedly(Return(false));
    }

    void selectedStatements(std::vector<ast::StatementPtr> rs)
    {
        for (auto r : rs)
            EXPECT_FCALL(isSelected(r)).WillRepeatedly(Return(true));
    }

    void expectRangeIs(ast::StatementRange range, ast::Statements stmts)
    {
        ASSERT_EQ(int(stmts.size()), boost::size(range));
        for (decltype(stmts.size()) i = 0; i != stmts.size(); ++i, range.pop_front())
            ASSERT_TRUE(stmts[i] == range.front()) << "stmts[" << i << "]";
    }
};

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_for_an_empty_function)
{
    ast::Function f{0, {}};

    auto stmts = findSelectedStatementsInFunction(f, [&](ast::StatementPtr s) { return isSelected(s); });
    ASSERT_TRUE(stmts.empty());
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_the_selected_range_of_statements)
{
    ast::Statements stmts = {stmt(), stmt(), stmt(), stmt()};
    ast::Function f{0, stmts}; // TODO: move to factory
    selectedStatements({ stmts[1], stmts[2] });

    auto found = findSelectedStatementsInFunction(f, [&](ast::StatementPtr s) { return isSelected(s); });

    expectRangeIs(found, { stmts[1], stmts[2] });
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_when_no_statement_are_selected)
{
    ast::Function f{0, {stmt(), stmt()}};
    EXPECT_FCALL(isSelected(_)).WillRepeatedly(Return(false));

    auto stmts = findSelectedStatementsInFunction(f, [&](ast::StatementPtr s) { return isSelected(s); });

    ASSERT_TRUE(stmts.empty());
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_selected_children_if_the_only_selected_statement_has_children)
{
    ast::Statements children = { stmt(), stmt(), stmt(), stmt() };
    ast::Statements stmts = { stmt(), stmt(children), stmt() };
    ast::Function f{0, { stmts }};
    selectedStatements({ stmts[1], children[1], children[2] });

    auto found = findSelectedStatementsInFunction(f, [&](ast::StatementPtr s) { return isSelected(s); });

    expectRangeIs(found, { children[1], children[2] });
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_the_only_selected_statement_if_it_has_no_children)
{
    ast::Statements stmts = { stmt(), stmt(), stmt() };
    ast::Function f{0, { stmts }};
    selectedStatements({ stmts[1] });

    auto found = findSelectedStatementsInFunction(f, [&](ast::StatementPtr s) { return isSelected(s); });

    expectRangeIs(found, { stmts[1] });
}

}
}
}
