#include <cppmanip/query/findVariablesDeclaredByAndUsedAfterStmts.hpp>
#include "../ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{

struct findVariablesDeclaredByAndUsedAfterStmtsTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
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

    clang::VarDecl *varDecl(unsigned n, StmtsWithScope stmts)
    {
        return clang::dyn_cast<clang::VarDecl>(clang::dyn_cast<clang::DeclStmt>(*boost::next(begin(stmts.range), n))->getSingleDecl());
    }

    void expectEqUnordered(std::vector<clang::VarDecl *> found, std::vector<clang::VarDecl *> expected)
    {
        ASSERT_EQ(expected.size(), found.size());
        std::sort(found.begin(), found.end());
        std::sort(expected.begin(), expected.end());
        ASSERT_TRUE(expected == found);
    }
};

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_no_variables_if_none_are_declared)
{
    declareGlobal("void f(); void g();");
    auto stmts = parseStmts("f(); g();");

    ASSERT_TRUE(findVariablesDeclaredByAndUsedAfterStmts(stmts.range, stmts.scope).empty());
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_variables_used_after_given_statements)
{
    declareGlobal("void f(int); void g(int);");
    auto stmts = parseStmts("int i = 1; int j = 2; int x = 3; f(j); g(i + j);");
    const auto INT_I = 0, INT_J = 1;

    auto found = findVariablesDeclaredByAndUsedAfterStmts(first(3, stmts.range), stmts.scope);
    expectEqUnordered(found, { varDecl(INT_I, stmts), varDecl(INT_J, stmts) });
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_only_return_variables_declared_by_given_statements)
{
    declareGlobal("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; int y = 2; int z = 3; int w = 4; f(x + y); g(z + w);");
    const auto INT_X = 0, INT_Y = 1, INT_Z = 2, INT_W = 3;

    auto checked = first(2, skip(2, stmts.range));
    auto found = findVariablesDeclaredByAndUsedAfterStmts(checked, stmts.scope);
    expectEqUnordered(found, { varDecl(INT_Z, stmts), varDecl(INT_W, stmts) });
}

}
}
