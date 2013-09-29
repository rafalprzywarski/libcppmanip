#include "findVariablesDeclaredByAndUsedAfterStmts.hpp"

namespace cppmanip
{
namespace query
{

std::vector<clang::VarDecl *> findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent)
{
    return std::vector<clang::VarDecl *>();
}

}
}
