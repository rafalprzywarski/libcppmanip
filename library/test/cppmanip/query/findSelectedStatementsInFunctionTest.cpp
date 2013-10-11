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
    std::function<bool(ast::StatementPtr)> selected(std::set<ast::StatementPtr> stmts)
    {
        return [stmts](ast::StatementPtr s) { return stmts.count(s) != 0; };
    }

    std::function<bool(ast::StatementPtr)> nothingSelected()
    {
        return selected({});
    }

    void expectRangeIs(ast::StatementRange range, ast::Statements stmts)
    {
        ASSERT_EQ(int(stmts.size()), boost::size(range));
        for (decltype(stmts.size()) i = 0; i != stmts.size(); ++i, range.pop_front())
            ASSERT_TRUE(stmts[i] == range.front()) << "stmts[" << i << "]";
    }

    void verifyFindSelectedStmtsInFunctionWithStmtsReturns(IsStatementSelected isSelected, ast::Statements stmts, ast::Statements expected)
    {
        ast::Function f{0, { stmts }}; // TODO: move to factory
        auto found = findSelectedStatementsInFunction(f, isSelected);
        expectRangeIs(found, expected);
    }
};

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_for_an_empty_function)
{
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(nothingSelected(), {}, {});
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_the_selected_range_of_statements)
{
    ast::Statements stmts = {stmt(), stmt(), stmt(), stmt()};
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(selected({ stmts[1], stmts[2] }), stmts, { stmts[1], stmts[2] });
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_when_no_statement_are_selected)
{
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(nothingSelected(), {stmt(), stmt()}, {});
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_selected_children_if_the_only_selected_statement_has_children)
{
    ast::Statements children = { stmt(), stmt(), stmt(), stmt() };
    ast::Statements stmts = { stmt(), stmt(children), stmt() };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(selected({ stmts[1], children[1], children[2] }), stmts, { children[1], children[2] });
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_the_only_selected_statement_if_it_has_no_children)
{
    ast::Statements stmts = { stmt(), stmt(), stmt() };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(selected({ stmts[1] }), stmts, { stmts[1] });
}

}
}
}
