#include <cppmanip/DefaultStatementLocator.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <clang/AST/Stmt.h>
#include <boost/next_prior.hpp>

using namespace testing;

namespace cppmanip
{

struct DefaultStatementLocatorTest : testing::Test
{
    MOCK_METHOD2(getStmtRange, LocationRange(clang::SourceManager&, clang::Stmt& ));

    std::unique_ptr<test::ParsedFunction> parsedFunction;
    clang::FunctionDecl *parsedFunctionDecl = nullptr;
    std::unique_ptr<DefaultStatementLocator> locator;

    void parseFunctionWithStmts(const std::string& stmts)
    {
        auto source = "void func__() {" + stmts + "}\n";
        parsedFunction.reset(new test::ParsedFunction(source));
        parsedFunctionDecl = parsedFunction->getDecl();
    }

    void createLocatorWithSelection(LocationRange selection)
    {
        locator.reset(new DefaultStatementLocator(
            [&](clang::SourceManager& sm, clang::Stmt& s) { return getStmtRange(sm, s); },
            selection));
    }

    clang::Stmt *stmtNo(int index)
    {
        return *boost::next(parsedFunctionDecl->getBody()->child_begin(), index);
    }

    void expectGetRangeForStmtNoAndReturn(int index, LocationRange range)
    {
        auto& sourceManager = parsedFunctionDecl->getASTContext().getSourceManager();
        EXPECT_CALL(*this, getStmtRange(Ref(sourceManager), Ref(*stmtNo(index))))
            .WillRepeatedly(Return(range));
    }

    void expectRangeIs(clang::StmtRange range, std::vector<clang::Stmt *> stmts)
    {
        ASSERT_EQ(stmts.size(), std::distance(begin(range), end(range)));
        for (decltype(stmts.size()) i = 0; i != stmts.size(); ++i, ++range)
            ASSERT_TRUE(stmts[i] == *range) << "stmts[" << i << "]";
    }

};

TEST_F(DefaultStatementLocatorTest, should_return_an_empty_range_for_an_empty_function)
{
    createLocatorWithSelection({ rowCol(1, 1), rowCol(1, 1) });
    parseFunctionWithStmts(" ");

    auto stmts = locator->findStatementsInFunction(*parsedFunctionDecl);
    ASSERT_TRUE(stmts.empty());
}

TEST_F(DefaultStatementLocatorTest, should_return_statements_overlapping_the_selection)
{
    createLocatorWithSelection({ rowCol(2, 0), rowCol(3, 4) });
    parseFunctionWithStmts("\n  int x;\n  int y;\n  int z;\n  int w;\n");
    const auto INT_X = 0, INT_Y = 1, INT_Z = 2, INT_W = 3;
    expectGetRangeForStmtNoAndReturn(INT_X, { rowCol(1, 2), rowCol(1, 8) });
    expectGetRangeForStmtNoAndReturn(INT_Y, { rowCol(2, 2), rowCol(2, 8) });
    expectGetRangeForStmtNoAndReturn(INT_Z, { rowCol(3, 2), rowCol(3, 8) });
    expectGetRangeForStmtNoAndReturn(INT_W, { rowCol(4, 2), rowCol(4, 8) });

    auto stmts = locator->findStatementsInFunction(*parsedFunctionDecl);

    expectRangeIs(stmts, { stmtNo(INT_Y), stmtNo(INT_Z) });
}

TEST_F(DefaultStatementLocatorTest, should_return_an_empty_range_when_no_statement_overlaps_the_selection)
{
    createLocatorWithSelection({ rowCol(0, 1), rowCol(0, 3) });
    parseFunctionWithStmts("\n  int a;\nint b;");
    const auto INT_A = 0, INT_B = 1;
    expectGetRangeForStmtNoAndReturn(INT_A, { rowCol(1, 2), rowCol(1, 8) });
    expectGetRangeForStmtNoAndReturn(INT_B, { rowCol(2, 0), rowCol(2, 6) });

    auto stmts = locator->findStatementsInFunction(*parsedFunctionDecl);

    ASSERT_TRUE(stmts.empty());
}

}
