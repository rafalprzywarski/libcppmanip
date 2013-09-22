#include <cppmanip/SourceExtractor.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <boost/scoped_ptr.hpp>

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

struct SourceExtractorTest : testing::TestWithParam<Stmt>
{
    boost::scoped_ptr<test::ParsedFunction> func;
    boost::scoped_ptr<SourceExtractor> extractor;
    std::string extraDeclarations;
    std::string parsedSource;

    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
        extractor.reset(new SourceExtractor(func->getASTContext().getSourceManager()));
        parsedSource = source;
    }

    void setExtraDeclarations(const std::string& decls)
    {
        extraDeclarations = decls;
    }

    clang::SourceRange getStmtRangeFromSourceExtractor(const std::string& stmt)
    {
        parse(extraDeclarations + " void f() { " + stmt + " }");
        return extractor->getCorrectSourceRange(**func->stmts());
    }

    void expectStmtRangeIs(clang::SourceRange range, const std::string& phrase)
    {
        auto phraseOffset = parsedSource.find(phrase);
        ASSERT_EQ(phraseOffset, extractor->getOffset(range.getBegin()));
        ASSERT_EQ(phraseOffset + phrase.length(), extractor->getOffset(range.getEnd()));
    }
};

TEST_P(SourceExtractorTest, should_find_correct_source_range_for_a_statement)
{
    setExtraDeclarations(GetParam().extraDecl);
    auto range = getStmtRangeFromSourceExtractor(GetParam().stmt);
    expectStmtRangeIs(range, GetParam().stmt);
}

INSTANTIATE_TEST_CASE_P(
    should_find_correct_source_range_for_all_statements,
    SourceExtractorTest,
    Values(
        Stmt("g(7);").withExtraDecl("void g(int);"),
        Stmt("int x ;"),
        Stmt("for (;;) g(7);").withExtraDecl("void g(int);")
));

}
}
