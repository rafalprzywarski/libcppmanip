#include <cppmanip/getStmtRange.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>

using namespace testing;

namespace cppmanip
{
namespace test
{

struct Stmt
{
    std::string stmt;
    std::string extraDecl;
    Stmt(const std::string& stmt) : stmt(stmt) { }
    Stmt& withExtraDecl(const std::string& decl)
    {
        extraDecl = decl;
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, const Stmt& s)
{
    os << "\"" << s.stmt << "\"";
    if (!s.extraDecl.empty())
        os << " with \"" << s.extraDecl << "\"";
    return os;
}

struct getStmtRangeTest : testing::TestWithParam<Stmt>
{
    std::unique_ptr<test::ParsedFunction> func;
    std::string extraDeclarations;
    std::string parsedSource;

    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
        parsedSource = source;
    }

    void setExtraDeclarations(const std::string& decls)
    {
        extraDeclarations = decls;
    }

    LocationRange getStmtRangeFromSourceExtractor(const std::string& stmt)
    {
        parse(extraDeclarations + " void dummy_function__() { " + stmt + " }");
        return getStmtRange(func->getDecl()->getASTContext().getSourceManager(), **func->stmts());
    }

    void expectStmtRangeIs(LocationRange range, const std::string& phrase)
    {
        auto phraseOffset = parsedSource.find(phrase);
        ASSERT_EQ(LocationRange(rowCol(0, phraseOffset), rowCol(0, phraseOffset + phrase.length())), range)
            << (*func->stmts())->getStmtClassName();
    }
};

TEST_P(getStmtRangeTest, should_find_correct_source_range_for_a_statement)
{
    setExtraDeclarations(GetParam().extraDecl);
    auto range = getStmtRangeFromSourceExtractor(GetParam().stmt);
    expectStmtRangeIs(range, GetParam().stmt);
}

INSTANTIATE_TEST_CASE_P(
    should_find_correct_source_range_for_all_statements,
    getStmtRangeTest,
    Values(
        Stmt("int x;"),
        Stmt("int x  ;"),
        Stmt("int z = 7;"),
        Stmt("int z = f();").withExtraDecl("int f();"),
        Stmt("float f(3.0f);"),

        Stmt("f(3, 4);").withExtraDecl("void f(int, int);")
));

}
}
