#ifndef CPPMANIP_050F6B6B2ADD47528D05772006AEF4C7_HPP
#define CPPMANIP_050F6B6B2ADD47528D05772006AEF4C7_HPP

#include "LocalVariableLocator.hpp"

namespace cppmanip
{

class NaiveLocalVariableLocator : public LocalVariableLocator
{
public:
    virtual Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts);
    virtual Variables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);
};

}
#endif // CPPMANIP_050F6B6B2ADD47528D05772006AEF4C7_HPP
