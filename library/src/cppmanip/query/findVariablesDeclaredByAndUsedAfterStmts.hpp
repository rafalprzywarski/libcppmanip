#ifndef FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#define FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#include <vector>
#include <clang/AST/StmtIterator.h>

namespace clang { class VarDecl; }

namespace cppmanip
{
namespace query
{

std::vector<clang::VarDecl *> findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);

}
}
#endif // FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
