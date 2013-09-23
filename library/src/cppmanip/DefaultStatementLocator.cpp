#include "DefaultStatementLocator.hpp"
#include <clang/AST/Stmt.h>

namespace cppmanip
{

clang::StmtRange DefaultStatementLocator::findStatementsInFunction(const clang::FunctionDecl& decl)
{
    return clang::StmtRange();
}

}
