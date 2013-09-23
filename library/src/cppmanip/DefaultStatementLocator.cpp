#include "DefaultStatementLocator.hpp"
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

clang::StmtRange DefaultStatementLocator::findStatementsInFunction(const clang::FunctionDecl& decl)
{
    auto first = decl.getBody()->child_begin();
    if (first == decl.getBody()->child_end())
        return clang::StmtRange();
    while (!getStmtRange(decl.getASTContext().getSourceManager(), **first).overlapsWith(selection))
        ++first;
    auto last = first;
    ++last;
    return clang::StmtRange(first, last);
}

}
