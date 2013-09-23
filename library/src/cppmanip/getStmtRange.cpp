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
            s->getStartLoc(),
            clang::Lexer::getLocForEndOfToken(s->getEndLoc(), 0, sourceManager, clang::LangOptions()));
        return false;
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
