#include <cppmanip/DefaultLocalVariableUsageDetector.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <clang/AST/Stmt.h>

namespace cppmanip
{

struct DefaultLocalVariableUsageDetectorTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    std::string extraDeclarations;
    DefaultLocalVariableUsageDetector detector;

    void declareFunctions(const std::string& functions)
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

    void expectEqUnordered(LocalVariableUsageDetector::Variables found, LocalVariableUsageDetector::Variables expected)
    {
        ASSERT_EQ(expected.size(), found.size());
        std::sort(found.begin(), found.end());
        std::sort(expected.begin(), expected.end());
        ASSERT_TRUE(expected == found);
    }
};

TEST_F(DefaultLocalVariableUsageDetectorTest, should_return_no_variables_of_none_are_used)
{
    declareFunctions("void f(); void g();");
    auto stmts = parseStmts("f(); g();");

    ASSERT_TRUE(detector.findLocalVariablesRequiredForStmts(stmts).empty());
}

TEST_F(DefaultLocalVariableUsageDetectorTest, should_return_variables_in_given_range)
{
    declareFunctions("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; int y = 2; f(x); g(y);");
    const auto INT_X = 0, INT_Y = 1;
    auto checked = skip(2, stmts);

    auto found = detector.findLocalVariablesRequiredForStmts(checked);
    expectEqUnordered(found, { varDecl(INT_X, stmts), varDecl(INT_Y, stmts) });
}

TEST_F(DefaultLocalVariableUsageDetectorTest, should_not_return_the_same_variable_twice)
{
    declareFunctions("void f(int); void g(int);");
    auto stmts = parseStmts("int x = 1; f(x); g(x);");
    auto checked = skip(1, stmts);

    auto found = detector.findLocalVariablesRequiredForStmts(checked);
    ASSERT_EQ(1u, found.size());
}

TEST_F(DefaultLocalVariableUsageDetectorTest, should_not_return_variables_declared_inside_the_given_range)
{
    declareFunctions("void f(int, int);");
    auto stmts = parseStmts("int x = 1; int y = 2; f(x, y); int z = 4; f(y, z);");
    const auto INT_X = 0;
    auto checked = skip(1, stmts);
    auto found = detector.findLocalVariablesRequiredForStmts(checked);
    expectEqUnordered(found, { varDecl(INT_X, stmts) });
}

}
