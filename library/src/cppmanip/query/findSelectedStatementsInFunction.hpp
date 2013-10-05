#ifndef CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#define CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{

clang::StmtRange findSelectedStatementsInFunction(
    const clang::FunctionDecl& decl, std::function<bool(clang::Stmt&)> isSelected);

}
}
#endif // CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
