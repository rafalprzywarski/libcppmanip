#include <cppmanip/DefaultLocalVariableUsageDetector.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <memory>

namespace cppmanip
{

struct DefaultLocalVariableUsageDetectorTest : testing::Test
{
    std::unique_ptr<test::ParsedFunction> func;
    std::string extraDeclarations;

    void declareFunctions(const std::string& functions)
    {
        extraDeclarations = functions;
    }

    clang::StmtRange parseStmts(const std::string& stmts)
    {
        func.reset(new test::ParsedFunction(extraDeclarations + "void func__() {" + stmts + "}"));
        return func->stmts();
    }
};

TEST_F(DefaultLocalVariableUsageDetectorTest, should_return_no_variables_of_none_are_used)
{
    DefaultLocalVariableUsageDetector detector;
    declareFunctions("void f(); void g();");
    auto stmts = parseStmts("f(); g();");

    ASSERT_TRUE(detector.findLocalVariablesRequiredForStmts(stmts).empty());
}

}
