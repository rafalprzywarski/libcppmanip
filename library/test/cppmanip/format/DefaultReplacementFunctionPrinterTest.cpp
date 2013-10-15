#include <cppmanip/format/DefaultReplacementFunctionPrinter.hpp>
#include "../query/AstFactories.hpp"
#include <gmock/gmock.h>
#include "../gtestdef.hpp"

using namespace testing;

namespace cppmanip
{
namespace format
{

using namespace std::placeholders;

struct DefaultReplacementFunctionPrinterTest : testing::Test
{
    MOCK_METHOD1(findLocalVariablesRequiredForStmts, ast::LocalVariables(ast::StatementRange));
    MOCK_METHOD2(printFunctionCall, std::string(const std::string&, const std::vector<std::string>&));
    MOCK_METHOD4(printFunctionDefinition, std::string(const std::string&, const std::string&, const std::vector<std::string>&, const std::string&));
    MOCK_METHOD1(formatStatements, std::string(ast::StatementRange));

    DefaultReplacementFunctionPrinter printer;

    DefaultReplacementFunctionPrinterTest()
        : printer(
            std::bind(&DefaultReplacementFunctionPrinterTest::findLocalVariablesRequiredForStmts, this, _1),
            std::bind(&DefaultReplacementFunctionPrinterTest::formatStatements, this, _1),
            std::bind(&DefaultReplacementFunctionPrinterTest::printFunctionCall, this, _1, _2),
            std::bind(&DefaultReplacementFunctionPrinterTest::printFunctionDefinition, this, _1, _2, _3, _4))
    {
    }
};

TEST_F(DefaultReplacementFunctionPrinterTest, x)
{
    using namespace query::test;
    ast::Statements stmts = { stmt() };
    ast::LocalVariables vars { varWithNameAndNameWithType("a", "T a"), varWithNameAndNameWithType("b", "U b") };
    std::vector<std::string> varNames { "a", "b" };
    std::vector<std::string> varDecls { "T a", "U b" };
    EXPECT_FCALL(findLocalVariablesRequiredForStmts(ast::StatementRange(stmts))).WillRepeatedly(Return(vars));
    EXPECT_FCALL(formatStatements(ast::StatementRange(stmts))).WillRepeatedly(Return("formatted"));
    EXPECT_FCALL(printFunctionCall("name", varNames)).WillRepeatedly(Return("call"));
    EXPECT_FCALL(printFunctionDefinition("void", "name", varDecls, "formatted")).WillRepeatedly(Return("definition"));
    auto replacement = printer.printFunctionFromStmts("name", stmts);
    ASSERT_EQ("call", replacement.call);
    ASSERT_EQ("definition", replacement.definition);
}

}
}
