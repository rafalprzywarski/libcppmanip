#ifndef CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#define CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP

// TODO: semicolon hack
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <stdexcept>
#include "OffsetRange.hpp"
#include "LocationRange.hpp"

namespace clang
{
class VarDecl;
}

namespace cppmanip
{

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }
    
    clang::SourceRange getCorrectSourceRange(const clang::FunctionDecl& node);
    clang::SourceRange getCorrectSourceRange(const clang::Stmt& node);
    clang::SourceRange getCorrectSourceRange(clang::StmtRange stmts);
    std::string getSource(clang::SourceRange range);
    unsigned getOffset(clang::SourceLocation loc);

private:
    clang::SourceManager& sourceManager;
    
    template <typename Node>
    clang::SourceRange getSpellingRange(const Node& n);

    unsigned getSourceLength(clang::SourceRange spelling, const clang::FunctionDecl& node);
    unsigned getSourceLength(clang::SourceRange spelling, const clang::Stmt& node);

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

}
#endif // CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
