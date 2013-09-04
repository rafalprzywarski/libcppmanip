#ifndef NAIVESTATEMENTLOCATOR_HPP
#define NAIVESTATEMENTLOCATOR_HPP
#include "StatementLocator.hpp"
#include "SourceExtractor.hpp"

class NaiveStatementLocator : public StatementLocator
{
public:
    NaiveStatementLocator(SourceExtractor& sourceExtractor, OffsetRange selection)
        : sourceExtractor(sourceExtractor), selection(selection) { }

    clang::ConstStmtRange findStatementsInFunction(const clang::FunctionDecl& decl)
    {
        if (functionDoesNotContainSelection(decl))
            return clang::ConstStmtRange();
        return findStatementsTouchingSelection(decl);
    }

private:
    SourceExtractor& sourceExtractor;
    OffsetRange selection;

    bool functionDoesNotContainSelection(const clang::FunctionDecl& f)
    {
        return !sourceExtractor.isLocationFromMainFile(f.getLocation()) || !f.hasBody();
    }

    clang::ConstStmtRange findStatementsTouchingSelection(const clang::FunctionDecl& func)
    {
        auto body = func.getBody();
        auto begin =
            std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return selectionOverlapsWithStmt(*s); });
        auto end =
            std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !selectionOverlapsWithStmt(*s); });
        return {begin, end};
    }

    bool selectionOverlapsWithStmt(const clang::Stmt& stmt)
    {
        return selection.overlapsWith(sourceExtractor.getOffsetRange(sourceExtractor.getCorrectSourceRange(stmt)));
    }
};

#endif // NAIVESTATEMENTLOCATOR_HPP
