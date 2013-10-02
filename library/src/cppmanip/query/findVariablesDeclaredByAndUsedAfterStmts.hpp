#ifndef FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#define FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
#include <vector>
#include <clang/AST/StmtIterator.h>
#include <cppmanip/ast/LocalVariable.hpp>

namespace cppmanip
{
namespace query
{

ast::LocalVariables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);

}
}
#endif // FINDVARIABLESDECLAREDBYANDUSEDAFTERSTMTS_HPP
