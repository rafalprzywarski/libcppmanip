#ifndef CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#define CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP

// TODO: semicolon hack
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <stdexcept>
#include "OffsetRange.hpp"

namespace clang
{
class VarDecl;
    
}

namespace detail
{
    
template <typename Node>
inline unsigned extraCharsHack(const Node&) { return 0; }

template <>
inline unsigned extraCharsHack<clang::Stmt>(const clang::Stmt&) { return 1; } // semicolon

}

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }
    
    template <typename Node>
    clang::SourceRange getCorrectSourceRange(const Node& node)
    {
        auto spelling = getSpellingRange(node);
        auto sourceLength = getSourceLength(spelling, node);
        return {spelling.getBegin(), spelling.getBegin().getLocWithOffset(sourceLength)};
    }

    clang::SourceRange getCorrectSourceRange(clang::StmtRange stmts);
    std::string getSource(clang::SourceRange range);
    OffsetRange getOffsetRange(clang::SourceRange r);
    unsigned getOffset(clang::SourceLocation loc);
    bool isLocationFromMainFile(clang::SourceLocation loc);
    std::string getVarName(const clang::VarDecl& d);
    std::string getVarDecl(const clang::VarDecl& d);

private:
    clang::SourceManager& sourceManager;
    
    template <typename Node>
    clang::SourceRange getSpellingRange(const Node& n)
    {
        auto r = clang::SourceRange(
            sourceManager.getSpellingLoc(n.getSourceRange().getBegin()),
            sourceManager.getSpellingLoc(n.getSourceRange().getEnd()));
        if (r.isInvalid())
            throw std::runtime_error("cannot get spelling range");
        return r;
    }

    template <typename Node>
    unsigned getSourceLength(clang::SourceRange spelling, const Node& node)
    {
        auto start = getOffset(spelling.getBegin());
        auto end = sourceManager.getDecomposedLoc(
            clang::Lexer::getLocForEndOfToken(spelling.getEnd(), 0, sourceManager, clang::LangOptions())).second;
        if (end < start)
            throw std::runtime_error("invalid decomposed range, probably because of macros");
        return end - start + detail::extraCharsHack(node);
    }

    unsigned getDistance(clang::SourceLocation from, clang::SourceLocation to)
    {
        return getOffset(to) - getOffset(from);
    }
    
    unsigned getLength(clang::SourceRange r)
    {
        return getDistance(r.getBegin(), r.getEnd());
    }
    
    const char *getSourceText(clang::SourceLocation loc);
};

#endif // CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
