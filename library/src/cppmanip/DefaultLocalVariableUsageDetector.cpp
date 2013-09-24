#include "DefaultLocalVariableUsageDetector.hpp"
#include <clang/AST/RecursiveASTVisitor.h>

namespace cppmanip
{

namespace
{

struct UsedVariablesVisitor : clang::RecursiveASTVisitor<UsedVariablesVisitor>
{
    LocalVariableUsageDetector::Variables used;

    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast<clang::VarDecl>(d->getDecl());
        if (var)
            used.push_back(var);
        return true;
    }
};

}

LocalVariableUsageDetector::Variables DefaultLocalVariableUsageDetector::findLocalVariablesRequiredForStmts(
    clang::StmtRange stmts)
{
    UsedVariablesVisitor v;
    for (auto s : stmts)
        v.TraverseStmt(s);
    return v.used;
}
LocalVariableUsageDetector::Variables DefaultLocalVariableUsageDetector::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, clang::Stmt& parent)
{
    return Variables();
}

}

