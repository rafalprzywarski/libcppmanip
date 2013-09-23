#include "getStmtRange.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Lex/Lexer.h>

namespace cppmanip
{

class StmtVisitor : public clang::RecursiveASTVisitor<StmtVisitor>
{
public:
    StmtVisitor(clang::SourceManager& sourceManager)
        : sourceManager(sourceManager) { }
    bool VisitDeclStmt(clang::DeclStmt *s)
    {
        range = toLocationRange(
            s->getLocStart(),
            clang::Lexer::getLocForEndOfToken(s->getLocEnd(), 0, sourceManager, clang::LangOptions()));
        return false;
    }
    bool VisitCallExpr(clang::CallExpr *s)
    {
        range = toLocationRange(
            s->getLocStart(),
            clang::Lexer::findLocationAfterToken(s->getLocEnd(), clang::tok::semi, sourceManager, clang::LangOptions(), false));
        return false;
    }
    bool VisitForStmt(clang::ForStmt *s)
    {
        if (!clang::isa<clang::NullStmt>(s->getBody()))
            range = toLocationRange(
                s->getLocStart(),
                clang::Lexer::findLocationAfterToken(s->getLocEnd(), clang::tok::semi, sourceManager, clang::LangOptions(), false));
        else
            range = toLocationRange(
                s->getLocStart(),
                clang::Lexer::getLocForEndOfToken(s->getLocEnd(), 0, sourceManager, clang::LangOptions()));
    }
    LocationRange getRange() const { return range; }
private:
    LocationRange range;
    clang::SourceManager& sourceManager;
    unsigned toZeroBased(unsigned n) { return n - 1; }
    LocationRange toLocationRange(clang::SourceLocation b, clang::SourceLocation e)
    {
        return LocationRange(
            rowCol(0, toZeroBased(sourceManager.getSpellingColumnNumber(b))),
            rowCol(0, toZeroBased(sourceManager.getSpellingColumnNumber(e))));
    }
};

LocationRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    StmtVisitor v(sourceManager);
    v.TraverseStmt(&stmt);
    return v.getRange();
}

}
