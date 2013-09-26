#ifndef CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#define CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
#include "LocationRange.hpp"
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Stmt.h>

namespace cppmanip
{

typedef std::function<LocationRange(clang::SourceManager&, clang::Stmt& )> GetStmtRange;
clang::StmtRange findStatementsInFunctionOverlappingSelection(
    const clang::FunctionDecl& decl, LocationRange selection, GetStmtRange getStmtRange);

}
#endif // CPPMANIP_9F771D23280342E49A56A67096A3B6E5_HPP
