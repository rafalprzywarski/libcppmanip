#include "SourceExtractor.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>

namespace cppmanip
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
    r.setBegin(getCorrectSourceRange(**stmts).getBegin());
    for (auto s : stmts)
        r.setEnd(getCorrectSourceRange(*s).getEnd());
    return r;
}
    
std::string SourceExtractor::getSource(clang::SourceRange range) 
{
    return std::string(getSourceText(range.getBegin()), getLength(range));
}

const char* SourceExtractor::getSourceText(clang::SourceLocation loc) 
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return text;
}

OffsetRange SourceExtractor::getOffsetRange(clang::SourceRange r)
{
    return {getOffset(r.getBegin()), getOffset(r.getEnd())};
}

unsigned int SourceExtractor::getOffset(clang::SourceLocation loc)
{
    return sourceManager.getFileOffset(loc);
}

bool SourceExtractor::isLocationFromMainFile(clang::SourceLocation loc)
{
    return sourceManager.isFromMainFile(loc);
}

std::string SourceExtractor::getVarName(const clang::VarDecl& d)
{
    return d.getNameAsString();
}

std::string SourceExtractor::getVarDecl(const clang::VarDecl& d)
{
    return d.getType().getAsString() + " " + d.getNameAsString();
}

clang::SourceRange SourceExtractor::getCorrectSourceRange(const clang::FunctionDecl& node)
{
    auto spelling = getSpellingRange(node);
    auto sourceLength = getSourceLength(spelling, node);
    return {spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength)};
}

clang::SourceRange SourceExtractor::getCorrectSourceRange(const clang::Stmt& node)
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

unsigned int SourceExtractor::getSourceLength(clang::SourceRange spelling, const clang::Stmt& node)
{
    auto start = getOffset(spelling.getBegin());
    auto end = sourceManager.getDecomposedLoc(
        clang::Lexer::getLocForEndOfToken(spelling.getEnd(), 0, sourceManager, clang::LangOptions())).second;
    if (end < start)
        throw std::runtime_error("invalid decomposed range, probably because of macros");
    return end - start + extraCharsHack(node);
}

}
