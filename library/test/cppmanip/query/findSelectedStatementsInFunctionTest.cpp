#include <cppmanip/query/findSelectedStatementsInFunction.hpp>
#include "../ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../gtestdef.hpp"
#include <clang/AST/Stmt.h>
#include <boost/next_prior.hpp>

using namespace testing;

namespace cppmanip
{
namespace query
{

struct findStatementsInFunctionOverlappingSelectionTest : testing::Test
{
    MOCK_METHOD1(isSelected, bool(clang::Stmt& ));

    std::unique_ptr<test::ParsedFunction> parsedFunction;
    clang::FunctionDecl *parsedFunctionDecl = nullptr;

    void parseFunctionWithStmts(const std::string& stmts)
    {
        auto source = "void func__() {" + stmts + "}\n";
        parsedFunction.reset(new test::ParsedFunction(source));
        parsedFunctionDecl = parsedFunction->getDecl();
    }

    clang::Stmt *stmtNo(int index)
    {
        return *boost::next(parsedFunctionDecl->getBody()->child_begin(), index);
    }

    void expectGetRangeForStmtNoAndReturn(int index, bool selected)
    {
    }

    void stmtSelection(std::map<clang::Stmt *, bool> rs)
    {
        for (auto r : rs)
            EXPECT_FCALL(isSelected(Ref(*r.first))).WillRepeatedly(Return(r.second));
    }

    void expectRangeIs(clang::StmtRange range, std::vector<clang::Stmt *> stmts)
    {
        ASSERT_EQ(stmts.size(), std::distance(begin(range), end(range)));
        for (decltype(stmts.size()) i = 0; i != stmts.size(); ++i, ++range)
            ASSERT_TRUE(stmts[i] == *range) << "stmts[" << i << "]";
    }

};

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_for_an_empty_function)
{
    parseFunctionWithStmts(" ");

    auto stmts = findSelectedStatementsInFunction(*parsedFunctionDecl, [&](clang::Stmt& s) { return isSelected(s); });
    ASSERT_TRUE(stmts.empty());
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_the_selected_range_of_statements)
{
    parseFunctionWithStmts("\n  int x;\n  int y;\n  int z;\n  int w;\n");
    const auto INT_X = 0, INT_Y = 1, INT_Z = 2, INT_W = 3;
    stmtSelection({
        { stmtNo(INT_X), false },
        { stmtNo(INT_Y), true },
        { stmtNo(INT_Z), true },
        { stmtNo(INT_W), false } });

    auto stmts = findSelectedStatementsInFunction(*parsedFunctionDecl, [&](clang::Stmt& s) { return isSelected(s); });

    expectRangeIs(stmts, { stmtNo(INT_Y), stmtNo(INT_Z) });
}

TEST_F(findStatementsInFunctionOverlappingSelectionTest, should_return_an_empty_range_when_no_statement_are_selected)
{
    parseFunctionWithStmts("\n  int a;\nint b;");
    EXPECT_FCALL(isSelected(_)).WillRepeatedly(Return(false));

    auto stmts = findSelectedStatementsInFunction(*parsedFunctionDecl, [&](clang::Stmt& s) { return isSelected(s); });

    ASSERT_TRUE(stmts.empty());
}

}
}
