#include "NaiveStatementLocator.hpp"

NaiveStatementLocator::NaiveStatementLocator(SourceExtractor& sourceExtractor, OffsetRange selection)
    : sourceExtractor(sourceExtractor), selection(selection) { }

clang::StmtRange NaiveStatementLocator::findStatementsInFunction(const clang::FunctionDecl& decl)
{
    if (functionDoesNotContainSelection(decl))
        return clang::StmtRange();
    return findStatementsTouchingSelection(decl);
}

bool NaiveStatementLocator::functionDoesNotContainSelection(const clang::FunctionDecl& f)
{
    return !sourceExtractor.isLocationFromMainFile(f.getLocation()) || !f.hasBody();
}

clang::StmtRange NaiveStatementLocator::findStatementsTouchingSelection(const clang::FunctionDecl& func)
{
    auto body = func.getBody();
    auto begin =
        std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return selectionOverlapsWithStmt(*s); });
    auto end =
        std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !selectionOverlapsWithStmt(*s); });
    return {begin, end};
}

bool NaiveStatementLocator::selectionOverlapsWithStmt(const clang::Stmt& stmt)
{
    return selection.overlapsWith(sourceExtractor.getOffsetRange(sourceExtractor.getCorrectSourceRange(stmt)));
}
