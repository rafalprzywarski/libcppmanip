#include "DefaultLocalVariableUsageDetector.hpp"

namespace cppmanip
{

LocalVariableUsageDetector::Variables DefaultLocalVariableUsageDetector::findLocalVariablesRequiredForStmts(
    clang::StmtRange stmts)
{
    return Variables();
}
LocalVariableUsageDetector::Variables DefaultLocalVariableUsageDetector::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, clang::Stmt& parent)
{
    return Variables();
}

}

