#include <cppmanip/query/findLocalVariablesRequiredForStmts.hpp>
#include <cppmanip/ast/LocalVariable.hpp>
#include "../ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{

struct findLocalVariablesRequiredForStmtsTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    std::string extraDeclarations;

    void declareGlobal(const std::string& functions)
    {
        extraDeclarations = functions;
    }

    clang::StmtRange parseStmts(const std::string& stmts)
    {
        auto source = extraDeclarations + "void func__() {" + stmts + "}";
        func.reset(new test::ParsedFunction(source));
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

    void expectEqUnordered(std::vector<ast::LocalVariable> found, std::vector<ast::LocalVariable> expected)
    {
        auto order = [](ast::LocalVariable left, ast::LocalVariable right) { return left.getNameWithType() < right.getNameWithType(); };
        std::sort(found.begin(), found.end(), order);
        std::sort(expected.begin(), expected.end(), order);
        expectEqOrdered(found, expected);
    }

    void expectEqOrdered(std::vector<ast::LocalVariable> found, std::vector<ast::LocalVariable> expected)
    {
        ASSERT_EQ(expected.size(), found.size());
        for (decltype(found.size()) i = 0; i < found.size(); ++i)
        {
            ASSERT_EQ(expected[i].getName(), found[i].getName());
            ASSERT_EQ(expected[i].getNameWithType(), found[i].getNameWithType());
        }
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
    const auto INT_X = 0, INT_Y = 1;
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
    const auto INT_X = 0;
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
    const auto INT_C = 0, INT_A = 1, INT_B = 2;
    auto found = findLocalVariablesRequiredForStmts(skip(3, stmts));
    expectEqOrdered(found, { { "c", "int c" }, { "a", "int a" }, { "b", "int b" } });
}

}
}
