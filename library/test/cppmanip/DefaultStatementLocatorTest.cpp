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

    std::unique_ptr<test::ParsedFunction> function;
    std::unique_ptr<DefaultStatementLocator> locator;

    void parseSourceAndCreateLocatorForSelection(const std::string& source, LocationRange selection)
    {
        function.reset(new test::ParsedFunction(source));
        locator.reset(new DefaultStatementLocator(
            [&](clang::SourceManager& sm, clang::Stmt& s) { return getStmtRange(sm, s); },
            selection));
    }

    clang::Stmt *nthStmt(int index)
    {
        return *boost::next(function->getDecl()->getBody()->child_begin(), index);
    }

    void expectGetRangeForStmtAndReturn(int index, LocationRange range)
    {
        auto& sourceManager = function->getDecl()->getASTContext().getSourceManager();
        EXPECT_CALL(*this, getStmtRange(Ref(sourceManager), Ref(*nthStmt(index))))
            .WillRepeatedly(Return(range));
    }

};

TEST_F(DefaultStatementLocatorTest, should_return_an_empty_range_for_an_empty_function)
{
    parseSourceAndCreateLocatorForSelection("void f() {\n \n}\n", { rowCol(1, 1), rowCol(1, 1) });
    auto stmts = locator->findStatementsInFunction(*function->getDecl());
    ASSERT_TRUE(stmts.empty());
}

TEST_F(DefaultStatementLocatorTest, should_return_the_statements_touching_the_selection)
{
    parseSourceAndCreateLocatorForSelection("void f() {\n  int x;\n  int y;\n  int z;\n  int w;\n}\n", { rowCol(2, 0), rowCol(3, 4) });
    expectGetRangeForStmtAndReturn(0, { rowCol(1, 2), rowCol(1, 8) });
    expectGetRangeForStmtAndReturn(1, { rowCol(2, 2), rowCol(2, 8) });
    expectGetRangeForStmtAndReturn(2, { rowCol(3, 2), rowCol(3, 8) });
    expectGetRangeForStmtAndReturn(3, { rowCol(4, 2), rowCol(4, 8) });
    auto stmts = locator->findStatementsInFunction(*function->getDecl());
    ASSERT_EQ(2u, std::distance(begin(stmts), end(stmts)));
    ASSERT_TRUE(*stmts == nthStmt(1));
    ASSERT_TRUE(*boost::next(stmts) == nthStmt(2));
}

TEST_F(DefaultStatementLocatorTest, should_return_an_empty_range_when_no_statement_touches_the_selection)
{
    parseSourceAndCreateLocatorForSelection("void f() {\n  int a;\nint b;}", { rowCol(0, 1), rowCol(0, 3) });
    expectGetRangeForStmtAndReturn(0, { rowCol(1, 2), rowCol(1, 8) });
    expectGetRangeForStmtAndReturn(1, { rowCol(2, 0), rowCol(2, 6) });
    auto stmts = locator->findStatementsInFunction(*function->getDecl());
    ASSERT_TRUE(stmts.empty());
}

}
