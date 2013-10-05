#include <cppmanip/query/findVariablesDeclaredByAndUsedAfterStmts.hpp>
#include "../ParsedFunction.hpp"
#include "LocalVariablesAssert.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{
namespace test
{
using namespace cppmanip::test;

struct findVariablesDeclaredByAndUsedAfterStmtsTest : testing::Test
{
    std::unique_ptr<ParsedFunction> func;
    std::string extraDeclarations;

    void declareGlobal(const std::string& functions)
    {
        extraDeclarations = functions;
    }

    struct StmtsWithScope
    {
        clang::StmtRange range;
        clang::Stmt& scope;
    };
    StmtsWithScope parseStmts(const std::string& stmts)
    {
        auto source = extraDeclarations + "void func__() {" + stmts + "}";
        func.reset(new test::ParsedFunction(source));
        return { func->stmts(), *func->getDecl()->getBody() };
    }

    clang::StmtRange first(unsigned n, clang::StmtRange r)
    {
        return { begin(r), boost::next(begin(r), n) };
    }

    clang::StmtRange skip(unsigned n, clang::StmtRange r)
    {
        return { boost::next(begin(r), n), end(r) };
    }

    ast::LocalVariablePtr var()
    {
        return std::make_shared<ast::LocalVariable>("", "");
    }

    ast::StatementPtr stmtWithUsedVars(ast::LocalVariables vars)
    {
        return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), ast::LocalVariables(), vars);
    }

    ast::StatementPtr stmtWithDeclaredVars(ast::LocalVariables declared)
    {
        return std::make_shared<ast::Statement>(nullptr, ast::SourceOffsetRange(0, 0), declared, ast::LocalVariables());
    }

    ast::StatementPtr stmt()
    {
        return stmtWithDeclaredVars({});
    }
};

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_no_variables_if_none_are_declared)
{
    ast::Statements stmts{ stmt(), stmt() };

    ASSERT_TRUE(findVariablesDeclaredByAndUsedAfterStmts(stmts, stmts).empty());
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_variables_used_after_given_statements)
{
    auto var1 = var(), var2 = var(), var3 = var();
    ast::Statements stmts{ stmtWithDeclaredVars({var1}), stmtWithDeclaredVars({var2, var3}), stmtWithUsedVars({var2}), stmtWithUsedVars({var1, var2}) };

    auto found = findVariablesDeclaredByAndUsedAfterStmts({stmts.begin(), stmts.begin() + 2}, stmts);
    expectEqUnordered(found, { var1, var2 });
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_only_return_variables_declared_by_given_statements)
{
    auto var1 = var(), var2 = var(), var3 = var(), var4 = var();
    ast::Statements stmts{
        stmtWithDeclaredVars({var1, var2}), stmtWithDeclaredVars({var3}), stmtWithDeclaredVars({var4}),
        stmtWithUsedVars({var1, var2}), stmtWithUsedVars({var3, var4}) };

    auto found = findVariablesDeclaredByAndUsedAfterStmts({stmts.begin() + 1, stmts.begin() + 3}, stmts);
    expectEqUnordered(found, { var3, var4 });
}

}
}
}
