#include "DefaultLocalVariableUsageDetector.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>

namespace cppmanip
{

namespace
{

struct UsedVariablesVisitor : clang::RecursiveASTVisitor<UsedVariablesVisitor>
{
    std::unordered_set<clang::VarDecl *> used;

    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast<clang::VarDecl>(d->getDecl());
        if (var)
            used.insert(var);
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
    return { begin(v.used), end(v.used) };
}
LocalVariableUsageDetector::Variables DefaultLocalVariableUsageDetector::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, clang::Stmt& parent)
{
    return Variables();
}

}

