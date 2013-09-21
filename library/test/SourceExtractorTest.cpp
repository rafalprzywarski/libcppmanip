#include <SourceExtractor.hpp>
#include "ParsedFunction.hpp"
#include <gtest/gtest.h>
#include <boost/scoped_ptr.hpp>

namespace cppmanip
{

struct SourceExtractorTest : testing::Test
{
    boost::scoped_ptr<test::ParsedFunction> func;
    boost::scoped_ptr<SourceExtractor> extractor;

    void parse(const std::string& source)
    {
        func.reset(new test::ParsedFunction(source));
        extractor.reset(new SourceExtractor(func->getASTContext().getSourceManager()));
    }
};

TEST_F(SourceExtractorTest, shouldFindCorrectSourceRangeForFunctionCalls)
{
    std::string source = "void g(int); void f() { g(7);/*end*/ }";
    parse(source);
    auto r = extractor->getCorrectSourceRange(**func->stmts());
    ASSERT_EQ(source.find("g(7)"), extractor->getOffset(r.getBegin()));
    ASSERT_EQ(source.find("/*end*/"), extractor->getOffset(r.getEnd()));
}

}
