#ifndef CPPMANIP_74A28F4CBA3943F3B3E1299FF722AA23_HPP
#define CPPMANIP_74A28F4CBA3943F3B3E1299FF722AA23_HPP
#include <vector>
#include <clang/AST/StmtIterator.h>

namespace clang
{
class VarDecl;
class FunctionDecl;
}

class LocalVariableLocator
{
public:
    typedef std::vector<clang::VarDecl *> Variables;
    virtual Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts) = 0;
    virtual Variables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& func) = 0;
};

#endif // CPPMANIP_74A28F4CBA3943F3B3E1299FF722AA23_HPP
