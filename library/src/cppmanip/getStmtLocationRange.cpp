#include "getStmtLocationRange.hpp"
#include <boost/optional.hpp>
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Lex/Lexer.h>

namespace cppmanip
{
namespace
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
        getRange(s);
        auto r = getStmtRange(sourceManager, *s->getBody());
        range->setEnd(r.getEnd());
        return false;
    }
    bool VisitCompoundAssignOperator(clang::CompoundAssignOperator *s)
    {
        getRangeTillSemicolon(s);
        return false;
    }
    bool VisitIfStmt(clang::IfStmt *s)
    {
        getRangeTillSemicolon(s);
        return false;
    }
    bool VisitCompoundStmt(clang::CompoundStmt *s)
    {
        getRange(s);
        return false;
    }
    bool VisitNullStmt(clang::NullStmt *s)
    {
        getRange(s);
        return false;
    }

    boost::optional<clang::SourceRange> getRange() const { return range; }
private:
    boost::optional<clang::SourceRange> range;
    clang::SourceManager& sourceManager;
    void getRange(clang::Stmt *s)
    {
        range = clang::SourceRange(
            s->getLocStart(),
            clang::Lexer::getLocForEndOfToken(s->getLocEnd(), 0, sourceManager, clang::LangOptions()));
    }
    void getRangeTillSemicolon(clang::Stmt *s)
    {
        range = clang::SourceRange(
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

}

clang::SourceRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    StmtVisitor v(sourceManager);
    v.TraverseStmt(&stmt);
    auto range = v.getRange();
    if (!range)
        throw cppmanip::ExtractMethodError(std::string("Unhandled statement"));
    return *range;
}

LocationRange getStmtLocationRange(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    return toLocationRange(sourceManager, getStmtRange(sourceManager, stmt));
}

}
