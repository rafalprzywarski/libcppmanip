#include "SourceExtractor.hpp"
#include <cppmanip/query/getStmtLocationRange.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>

namespace cppmanip
{
namespace legacy
{

namespace
{

unsigned extraCharsHack(const clang::Stmt& s) // semicolon
{
    return clang::isa<clang::ForStmt>(s) || clang::isa<clang::CallExpr>(s);
}

}


clang::SourceRange SourceExtractor::getCorrectSourceRange(clang::StmtRange stmts)
{
    clang::SourceRange r;
    r.setBegin(query::getStmtRange(sourceManager, **stmts).getBegin());
    for (auto s : stmts)
        r.setEnd(query::getStmtRange(sourceManager, *s).getEnd());
    return r;
}
    
std::string SourceExtractor::getSource(clang::SourceRange range) 
{
    return std::string(getSourceText(range.getBegin()), getLength(range));
}

std::string SourceExtractor::getSource(clang::StmtRange range)
{
    return getSource(getCorrectSourceRange(range));
}

const char* SourceExtractor::getSourceText(clang::SourceLocation loc) 
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return text;
}

unsigned int SourceExtractor::getOffset(clang::SourceLocation loc)
{
    return sourceManager.getFileOffset(loc);
}

clang::SourceRange SourceExtractor::getCorrectSourceRange(const clang::FunctionDecl& node)
{
    auto spelling = getSpellingRange(node);
    auto sourceLength = getSourceLength(spelling, node);
    return {spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength)};
}

template <typename Node>
clang::SourceRange SourceExtractor::getSpellingRange(const Node& n)
{
    auto r = clang::SourceRange(
        sourceManager.getSpellingLoc(n.getSourceRange().getBegin()),
        sourceManager.getSpellingLoc(n.getSourceRange().getEnd()));
    if (r.isInvalid())
        throw std::runtime_error("cannot get spelling range");
    return r;
}

unsigned int SourceExtractor::getSourceLength(clang::SourceRange spelling, const clang::FunctionDecl& node)
{
    auto start = getOffset(spelling.getBegin());
    auto end = getOffset(spelling.getEnd());
    if (end < start)
        throw std::runtime_error("invalid decomposed range, probably because of macros");
    return end - start;
}

}
}
