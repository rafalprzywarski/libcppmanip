#include <cppmanip/query/findLocalVariablesRequiredForStmts.hpp>
#include <cppmanip/ast/LocalVariable.hpp>
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

struct findLocalVariablesRequiredForStmtsTest : testing::Test
{
    std::unique_ptr<ParsedFunction> func;
    std::string extraDeclarations;

    void declareGlobal(const std::string& functions)
    {
        extraDeclarations = functions;
    }

    clang::StmtRange parseStmts(const std::string& stmts)
    {
        auto source = extraDeclarations + "void func__() {" + stmts + "}";
        func.reset(new ParsedFunction(source));
        return func->stmts();
    }

    clang::StmtRange skip(unsigned n, clang::StmtRange r)
    {
        return { boost::next(begin(r), n), end(r) };
    }

    clang::VarDecl *varDecl(unsigned n, clang::StmtRange stmts)
    {
        return clang::dyn_cast<clang::VarDecl>(clang::dyn_cast<clang::DeclStmt>(*boost::next(begin(stmts), n))->getSingleDecl());
    }
};

TEST_F(findLocalVariablesRequiredForStmtsTest, should_return_no_variables_of_none_are_used)
{
    declareGlobal("void f(); void g();");
    auto stmts = parseStmts("f(); g();");

    ASSERT_TRUE(findLocalVariablesRequiredForStmts(stmts).empty());
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_return_variables_in_given_range)
{
    declareGlobal("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; int y = 2; f(x); g(y);");
    auto checked = skip(2, stmts);

    auto found = findLocalVariablesRequiredForStmts(checked);
    expectEqUnordered(found, { { "x", "int x" }, { "y", "int y" } });
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_not_return_the_same_variable_twice)
{
    declareGlobal("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; f(x); g(x);");
    auto checked = skip(1, stmts);

    auto found = findLocalVariablesRequiredForStmts(checked);
    ASSERT_EQ(1u, found.size());
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_not_return_variables_declared_inside_the_given_range)
{
    declareGlobal("void f(int, int);");
    auto stmts = parseStmts("int x = 1; int y = 2; f(x, y); int z = 4; f(y, z);");
    auto checked = skip(1, stmts);
    auto found = findLocalVariablesRequiredForStmts(checked);
    expectEqUnordered(found, { { "x", "int x" } });
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_not_return_global_variables)
{
    declareGlobal("int g;");
    auto stmts = parseStmts("int x = g;");
    ASSERT_TRUE(findLocalVariablesRequiredForStmts(stmts).empty());
}

TEST_F(findLocalVariablesRequiredForStmtsTest, should_return_variables_in_order_of_their_declaration)
{
    declareGlobal("void f(int, int, int);");
    auto stmts = parseStmts("int c(0); int a(0); int b(0); f(b, c, a);");
    auto found = findLocalVariablesRequiredForStmts(skip(3, stmts));
    expectEqOrdered(found, { { "c", "int c" }, { "a", "int a" }, { "b", "int b" } });
}

}
}
}
