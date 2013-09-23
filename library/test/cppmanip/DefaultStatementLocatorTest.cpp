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
            std::bind(&DefaultStatementLocatorTest::getStmtRange, this, std::placeholders::_1, std::placeholders::_2),
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

TEST_F(DefaultStatementLocatorTest, should_return_the_statement_touching_the_selection)
{
    parseSourceAndCreateLocatorForSelection("void f() {\n  int x;\n  int y;\n  int z;\n}\n", { rowCol(2, 0), rowCol(2, 4) });
    expectGetRangeForStmtAndReturn(0, { rowCol(1, 2), rowCol(1, 8) });
    expectGetRangeForStmtAndReturn(1, { rowCol(2, 2), rowCol(2, 8) });
    expectGetRangeForStmtAndReturn(2, { rowCol(3, 2), rowCol(3, 8) });
    auto stmts = locator->findStatementsInFunction(*function->getDecl());
    ASSERT_EQ(1u, std::distance(begin(stmts), end(stmts)));
    ASSERT_TRUE(*stmts == nthStmt(1));
}

}
