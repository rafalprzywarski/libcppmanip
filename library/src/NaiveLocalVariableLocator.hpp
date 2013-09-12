#ifndef NAIVELOCALVARIABLELOCATOR_HPP
#define NAIVELOCALVARIABLELOCATOR_HPP

#include "LocalVariableLocator.hpp"

class NaiveLocalVariableLocator : public LocalVariableLocator
{
public:
    virtual Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts);
    virtual Variables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& func);
};

#endif // NAIVELOCALVARIABLELOCATOR_HPP
