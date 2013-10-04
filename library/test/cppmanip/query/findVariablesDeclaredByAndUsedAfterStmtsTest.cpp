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

    auto found = findVariablesDeclaredByAndUsedAfterStmts(first(3, stmts.range), stmts.scope);
    expectEqUnordered(found, { { "i", "int i" }, { "j", "int j" } });
}

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_only_return_variables_declared_by_given_statements)
{
    declareGlobal("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; int y = 2; int z = 3; int w = 4; f(x + y); g(z + w);");

    auto checked = first(2, skip(2, stmts.range));
    auto found = findVariablesDeclaredByAndUsedAfterStmts(checked, stmts.scope);
    expectEqUnordered(found, { { "z", "int z" }, { "w", "int w" } });
}

}
}
}
