#ifndef CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#define CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#include <vector>
#include <clang/AST/StmtIterator.h>

namespace clang { class VarDecl; }

namespace cppmanip
{

class DefaultLocalVariableUsageDetector
{
public:
    std::vector<clang::VarDecl *> findLocalVariablesRequiredForStmts(clang::StmtRange stmts);
    std::vector<clang::VarDecl *> findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);
};

}
#endif // CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
