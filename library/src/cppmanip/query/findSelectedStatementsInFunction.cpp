#include "findSelectedStatementsInFunction.hpp"
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
{

clang::StmtRange findSelectedStatementsInFunction(
    const clang::FunctionDecl& decl, std::function<bool(clang::Stmt&)> isSelected)
{
    auto first = std::find_if(
        decl.getBody()->child_begin(), decl.getBody()->child_end(),
        [&](clang::Stmt *s) { return isSelected(*s); });
    auto last = std::find_if(
        first, decl.getBody()->child_end(),
        [&](clang::Stmt *s) { return !isSelected(*s); });
    return clang::StmtRange(first, last);
}

}
}
