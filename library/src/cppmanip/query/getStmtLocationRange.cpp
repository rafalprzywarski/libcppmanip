#include "getStmtLocationRange.hpp"
#include <boost/optional.hpp>
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/StmtVisitor.h>
#include <clang/Lex/Lexer.h>

namespace cppmanip
{
namespace query
{
namespace
{

class StmtVisitor : public clang::StmtVisitor<StmtVisitor, boost::optional<clang::SourceRange>>
{
public:
    StmtVisitor(clang::SourceManager& sourceManager)
        : sourceManager(sourceManager) { }
    clang::SourceRange VisitDeclStmt(clang::DeclStmt *s)
    {
        return getRange(s);
    }
    clang::SourceRange VisitExpr(clang::Expr *s)
    {
        return getRangeTillSemicolon(s);
    }
    clang::SourceRange VisitForStmt(clang::ForStmt *s)
    {
        auto start = getRange(s);
        auto end = getStmtRange(sourceManager, *s->getBody());
        return { start.getBegin(), end.getEnd() };
    }
    clang::SourceRange VisitIfStmt(clang::IfStmt *s)
    {
        return getRangeTillSemicolon(s);
    }
    clang::SourceRange VisitCompoundStmt(clang::CompoundStmt *s)
    {
        return getRange(s);
    }
    clang::SourceRange VisitNullStmt(clang::NullStmt *s)
    {
        return getRange(s);
    }
private:
    clang::SourceManager& sourceManager;
    clang::SourceRange getRange(clang::Stmt *s)
    {
        return clang::SourceRange(
            s->getLocStart(),
            clang::Lexer::getLocForEndOfToken(s->getLocEnd(), 0, sourceManager, clang::LangOptions()));
    }
    clang::SourceRange getRangeTillSemicolon(clang::Stmt *s)
    {
        return clang::SourceRange(
            s->getLocStart(),
            clang::Lexer::findLocationAfterToken(s->getLocEnd(), clang::tok::semi, sourceManager, clang::LangOptions(), false));
    }
};

unsigned toZeroBased(unsigned n)
{
    return n - 1;
}

SourceLocation toRowCol(clang::SourceManager& sm, clang::SourceLocation l)
{
    return rowCol(
        toZeroBased(sm.getSpellingLineNumber(l)),
        toZeroBased(sm.getSpellingColumnNumber(l)));
}

LocationRange toLocationRange(clang::SourceManager& sm, clang::SourceRange r)
{
    return LocationRange(toRowCol(sm, r.getBegin()), toRowCol(sm, r.getEnd()));
}

clang::StmtRange last(clang::StmtRange r)
{
    clang::StmtRange last;
    do last = r++;
    while (!r.empty());
    return last;
}

}

clang::SourceRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    StmtVisitor v(sourceManager);
    auto range = v.Visit(&stmt);
    if (!range)
        throw cppmanip::ExtractMethodError(std::string("Unhandled statement"));
    return *range;
}

LocationRange getStmtLocationRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    return toLocationRange(sourceManager, getStmtRange(sourceManager, stmt));
}

clang::SourceRange getStmtsRange(clang::SourceManager& sourceManager, clang::StmtRange stmts)
{
    return { getStmtRange(sourceManager, **stmts).getBegin(), getStmtRange(sourceManager, **last(stmts)).getEnd() };
}

LocationRange getStmtsLocationRange(clang::SourceManager& sourceManager, clang::StmtRange stmts)
{
    return toLocationRange(sourceManager, getStmtsRange(sourceManager, stmts));
}

}
}
