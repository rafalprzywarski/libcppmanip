#ifndef NAIVELOCALVARIABLELOCATOR_HPP
#define NAIVELOCALVARIABLELOCATOR_HPP

#include "LocalVariableLocator.hpp"

class NaiveLocalVariableLocator : public LocalVariableLocator
{
public:
    virtual Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts);
};

#endif // NAIVELOCALVARIABLELOCATOR_HPP
