#ifndef SOURCEEXTRACTOR_HPP
#define SOURCEEXTRACTOR_HPP

// TODO: semicolon hack
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <stdexcept>

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

    clang::SourceRange getCorrectSourceRange(clang::ConstStmtRange stmts);
    std::string getSource(clang::ConstStmtRange stmts);

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
        auto start = locOffset(spelling.getBegin());
        auto end = sourceManager.getDecomposedLoc(
            clang::Lexer::getLocForEndOfToken(spelling.getEnd(), 0, sourceManager, clang::LangOptions())).second;
        if (end < start)
            throw std::runtime_error("invalid decomposed range, probably because of macros");
        return end - start + detail::extraCharsHack(node);
    }

    unsigned locOffset(clang::SourceLocation loc)
    {
        return sourceManager.getFileOffset(loc);
    }
    
    unsigned locDistance(clang::SourceLocation from, clang::SourceLocation to)
    {
        return locOffset(to) - locOffset(from);
    }
    
    unsigned rangeLength(clang::SourceRange r)
    {
        return locDistance(r.getBegin(), r.getEnd());
    }
    
    const char *getText(clang::SourceLocation loc);
};

#endif // SOURCEEXTRACTOR_HPP
