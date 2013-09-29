#include "findStatementsInFunctionOverlappingSelection.hpp"
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
{

clang::StmtRange findStatementsInFunctionOverlappingSelection(
    const clang::FunctionDecl& decl, LocationRange selection, GetStmtRange getStmtRange)
{
    auto first = std::find_if(
        decl.getBody()->child_begin(), decl.getBody()->child_end(),
        [&](clang::Stmt *s) { return getStmtRange(decl.getASTContext().getSourceManager(), *s).overlapsWith(selection); });
    auto last = std::find_if(
        first, decl.getBody()->child_end(),
        [&](clang::Stmt *s) { return !getStmtRange(decl.getASTContext().getSourceManager(), *s).overlapsWith(selection); });
    return clang::StmtRange(first, last);
}

}
}
