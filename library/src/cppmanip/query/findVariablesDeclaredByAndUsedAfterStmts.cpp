#include "findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>

namespace cppmanip
{
namespace query
{
namespace
{

class UsedVariablesVisitor : public clang::RecursiveASTVisitor<UsedVariablesVisitor>
{
public:
    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast<clang::VarDecl>(d->getDecl());
        if (var)
            used.insert(var);
        return true;
    }

    std::vector<clang::VarDecl *> getUsed() const { return { begin(used), end(used) }; }
private:
    std::unordered_set<clang::VarDecl *> used;
};

}

std::vector<clang::VarDecl *> findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent)
{
    UsedVariablesVisitor v;
    for (auto stmt : clang::StmtRange(end(stmts), parent.child_end()))
        v.TraverseStmt(stmt);
    return v.getUsed();
}

}
}
