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

    void expectEqUnordered(std::vector<clang::VarDecl *> found, std::vector<clang::VarDecl *> expected)
    {
        ASSERT_EQ(expected.size(), found.size());
        std::sort(found.begin(), found.end());
        std::sort(expected.begin(), expected.end());
        ASSERT_TRUE(expected == found);
    }

    void expectEqOrdered(std::vector<clang::VarDecl *> found, std::vector<clang::VarDecl *> expected)
    {
        ASSERT_EQ(expected.size(), found.size());
        ASSERT_TRUE(expected == found);
    }
};

TEST_F(findVariablesDeclaredByAndUsedAfterStmtsTest, should_return_no_variables_of_none_are_declared)
{
    declareGlobal("void f(); void g();");
    auto stmts = parseStmts("f(); g();");

    ASSERT_TRUE(findVariablesDeclaredByAndUsedAfterStmts(stmts, *func->getDecl()->getBody()).empty());
}

}
}
