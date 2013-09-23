#include "NaiveStatementLocator.hpp"
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

NaiveStatementLocator::NaiveStatementLocator(LocationRange selection)
    : selection(selection) { }

clang::StmtRange NaiveStatementLocator::findStatementsInFunction(const clang::FunctionDecl& decl)
{
    return findStatementsTouchingSelection(decl);
}

clang::StmtRange NaiveStatementLocator::findStatementsTouchingSelection(const clang::FunctionDecl& func)
{
    SourceExtractor sourceExtractor(func.getASTContext().getSourceManager());
    auto body = func.getBody();
    auto begin =
        std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return selectionOverlapsWithStmt(*s, sourceExtractor); });
    auto end =
        std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !selectionOverlapsWithStmt(*s, sourceExtractor); });
    return {begin, end};
}

bool NaiveStatementLocator::selectionOverlapsWithStmt(const clang::Stmt& stmt, SourceExtractor& sourceExtractor)
{
    return selection.overlapsWith(sourceExtractor.getLocationRange(sourceExtractor.getCorrectSourceRange(stmt)));
}

}
