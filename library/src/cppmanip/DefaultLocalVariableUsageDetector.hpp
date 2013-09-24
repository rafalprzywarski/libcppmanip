#ifndef CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#define CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
#include "LocalVariableUsageDetector.hpp"

namespace cppmanip
{

class DefaultLocalVariableUsageDetector : public LocalVariableUsageDetector
{
public:
    Variables findLocalVariablesRequiredForStmts(clang::StmtRange stmts);
    Variables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);
};

}
#endif // CPPMANIP_0ACCB425139A4912A691719B3EE19B55_HPP
