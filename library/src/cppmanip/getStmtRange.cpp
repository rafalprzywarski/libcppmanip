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
        getRange(s);
        return false;
    }
    bool VisitCallExpr(clang::CallExpr *s)
    {
        getRangeTillSemicolon(s);
        return false;
    }
    bool VisitForStmt(clang::ForStmt *s)
    {
        if (!clang::isa<clang::NullStmt>(s->getBody()))
            getRangeTillSemicolon(s);
        else
            getRange(s);
        return false;
    }
    bool VisitCompoundAssignOperator(clang::CompoundAssignOperator *s)
    {
        getRangeTillSemicolon(s);
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
            rowCol(toZeroBased(sourceManager.getSpellingLineNumber(b)), toZeroBased(sourceManager.getSpellingColumnNumber(b))),
            rowCol(toZeroBased(sourceManager.getSpellingLineNumber(e)), toZeroBased(sourceManager.getSpellingColumnNumber(e))));
    }
    void getRange(clang::Stmt *s)
    {
        range = toLocationRange(
            s->getLocStart(),
            clang::Lexer::getLocForEndOfToken(s->getLocEnd(), 0, sourceManager, clang::LangOptions()));
    }
    void getRangeTillSemicolon(clang::Stmt *s)
    {
        range = toLocationRange(
            s->getLocStart(),
            clang::Lexer::findLocationAfterToken(s->getLocEnd(), clang::tok::semi, sourceManager, clang::LangOptions(), false));
    }
};

LocationRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    StmtVisitor v(sourceManager);
    v.TraverseStmt(&stmt);
    return v.getRange();
}

}
