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

struct findSelectedStatementsInFunctionTest : testing::Test
{
    IsStatementSelected implicitlySelected(std::set<ast::StatementPtr> stmts)
    {
        return [stmts](ast::StatementPtr s) { return stmts.count(s) != 0 ? StatementSelected::IMPLICITLY : StatementSelected::NO; };
    }

    IsStatementSelected explicitlySelected(ast::StatementPtr stmt)
    {
        return [stmt](ast::StatementPtr s) { return stmt == s ? StatementSelected::EXPLICITLY : StatementSelected::NO; };
    }

    IsStatementSelected nothingSelected()
    {
        return implicitlySelected({});
    }

    void expectRangeIs(ast::StatementRange range, ast::Statements stmts, std::string name)
    {
        ASSERT_EQ(int(stmts.size()), boost::size(range)) << name;
        for (decltype(stmts.size()) i = 0; i != stmts.size(); ++i, range.pop_front())
            ASSERT_TRUE(stmts[i] == range.front()) << name << " stmts[" << i << "]";
    }

    void verifyFindSelectedStmtsInFunctionWithStmtsReturns(IsStatementSelected isSelected, ast::Statements stmts, ast::Statements expected)
    {
        verifyFindSelectedStmtsInFunctionWithStmtsReturnsStmtsInScope(isSelected, stmts, expected, stmts);
    }

    void verifyFindSelectedStmtsInFunctionWithStmtsReturnsStmtsInScope(IsStatementSelected isSelected, ast::Statements stmts, ast::Statements expected, ast::Statements scope)
    {
        auto f = functionWithStmts(stmts);
        auto found = findSelectedStatementsInFunction(*f, isSelected);
        expectRangeIs(found.getRange(), expected, "range");
        expectRangeIs(found.getScope(), scope, "scope");
    }
};

TEST_F(findSelectedStatementsInFunctionTest, should_return_an_empty_range_for_an_empty_function)
{
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(nothingSelected(), {}, {});
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_the_selected_range_of_statements)
{
    ast::Statements stmts = {stmt(), stmt(), stmt(), stmt()};
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(implicitlySelected({ stmts[1], stmts[2] }), stmts, { stmts[1], stmts[2] });
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_an_empty_range_when_no_statement_are_selected)
{
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(nothingSelected(), {stmt(), stmt()}, {});
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_selected_children_if_the_only_selected_statement_has_children)
{
    ast::Statements children = { stmt(), stmt(), stmt(), stmt() };
    ast::Statements stmts = { stmt(), stmt({ children }), stmt() };
    verifyFindSelectedStmtsInFunctionWithStmtsReturnsStmtsInScope(implicitlySelected({ stmts[1], children[1], children[2] }), stmts, { children[1], children[2] }, children);
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_the_only_selected_statement_if_it_has_no_children)
{
    ast::Statements stmts = { stmt(), stmt(), stmt() };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(implicitlySelected({ stmts[1] }), stmts, { stmts[1] });
}

TEST_F(findSelectedStatementsInFunctionTest, should_traverse_each_child_group)
{
    std::vector<ast::Statements> children = { { stmt() }, { stmt() } };
    ast::Statements stmts = { stmt(children) };
    verifyFindSelectedStmtsInFunctionWithStmtsReturnsStmtsInScope(implicitlySelected({ stmts[0], children[1][0] }), stmts, { children[1][0] }, children[1]);
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_parent_statement_if_its_children_are_not_selected)
{
    std::vector<ast::Statements> children = { { stmt() }, { stmt() } };
    ast::Statements stmts = { stmt(children) };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(implicitlySelected({ stmts[0] }), stmts, { stmts[0] });
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_parent_statement_if_both_its_children_groups_are_partially_selected)
{
    std::vector<ast::Statements> children = { { stmt(), stmt() }, { stmt(), stmt() } };
    ast::Statements stmts = { stmt(children) };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(implicitlySelected({ stmts[0], children[0][1], children[1][0] }), stmts, { stmts[0] });
}

TEST_F(findSelectedStatementsInFunctionTest, should_return_explicitly_selected_parent)
{
    ast::Statements stmts = { stmt() };
    verifyFindSelectedStmtsInFunctionWithStmtsReturns(explicitlySelected(stmts[0]), stmts, { stmts[0] });
}

}
}
}
