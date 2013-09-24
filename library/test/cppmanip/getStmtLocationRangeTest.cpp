#include <cppmanip/getStmtLocationRange.hpp>
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

struct getStmtLocationRangeTest : testing::TestWithParam<Stmt>
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

    LocationRange getRangeFromSource(const std::string& source)
    {
        parse(source);
        return getStmtLocationRange(func->getDecl()->getASTContext().getSourceManager(), **func->stmts());
    }

    LocationRange getRageFromStmt(const std::string& stmt)
    {
        return getRangeFromSource(extraDeclarations + " void dummy_function__() { " + stmt + " }");
    }

    void expectStmtRangeIs(LocationRange range, const std::string& phrase)
    {
        auto phraseOffset = parsedSource.find(phrase);
        ASSERT_EQ(LocationRange(rowCol(0, phraseOffset), rowCol(0, phraseOffset + phrase.length())), range)
            << (*func->stmts())->getStmtClassName();
    }
};

TEST_F(getStmtLocationRangeTest, should_handle_multiline_statements)
{
    auto range = getRangeFromSource("void dummy_function__() {\n  int\n x;\n}");
    ASSERT_EQ(LocationRange(rowCol(1, 2), rowCol(2, 3)), range);
}

TEST_P(getStmtLocationRangeTest, should_find_correct_source_range_for_a_statement)
{
    setExtraDeclarations(GetParam().extraDecl);
    auto range = getRageFromStmt(GetParam().stmt);
    expectStmtRangeIs(range, GetParam().stmt);
}

INSTANTIATE_TEST_CASE_P(
    should_find_correct_source_range_for_all_statements,
    getStmtLocationRangeTest,
    Values(
        Stmt("int x;"),
        Stmt("int x  ;"),
        Stmt("int z = 7;"),
        Stmt("int z = f();").withExtraDecl("int f();"),
        Stmt("float f(3.0f);"),

        Stmt("f(3, 4);").withExtraDecl("void f(int, int);"),

        Stmt("o.f();").withExtraDecl("struct O { void f(); } o;"),

        Stmt("for (;;);"),
        Stmt("for (;;) f();").withExtraDecl("void f();"),

        Stmt("i += 4;").withExtraDecl("int i;")
));

}
}
