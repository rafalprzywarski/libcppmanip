#ifndef LOCALVARIABLELOCATOR_HPP
#define LOCALVARIABLELOCATOR_HPP
#include <vector>
#include <clang/AST/StmtIterator.h>

namespace clang
{
class VarDecl;
}

class LocalVariableLocator
{
public:
    typedef std::vector<clang::VarDecl *> Variables;
    virtual Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts) = 0;
};

#endif // LOCALVARIABLELOCATOR_HPP
