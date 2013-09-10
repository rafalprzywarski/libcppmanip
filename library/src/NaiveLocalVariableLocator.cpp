#include "NaiveLocalVariableLocator.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>

namespace
{

class DeclRefExprVisitor : public clang::RecursiveASTVisitor<DeclRefExprVisitor>
{
public:
    typedef std::vector<clang::VarDecl *> Variables;

    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast_or_null<clang::VarDecl>(d->getDecl());
        if (!var || alreadyFound(var) || isGlobal(var))
            return true;
        variables.push_back(var);
        return true;
    }

    bool VisitVarDecl(clang::VarDecl *d)
    {
        notRequiredDecls.insert(d);
        return true;
    }

    virtual void traverseStmts(clang::StmtRange stmts)
    {
        for (auto& s : stmts)
            TraverseStmt(s);
    }

    Variables getVariables() const
    {
        Variables v;
        std::remove_copy_if(
            variables.begin(), variables.end(), std::back_inserter(v),
            [&](clang::VarDecl *d) { return notRequiredDecls.count(d); });
        return v;
    }
private:
    Variables variables;
    std::unordered_set<clang::VarDecl *> notRequiredDecls;

    bool alreadyFound(clang::VarDecl *var)
    {
        return std::find(variables.begin(), variables.end(), var) != variables.end();
    }

    bool isGlobal(clang::VarDecl *var)
    {
        return var->getParentFunctionOrMethod() == nullptr;
    }
};

}

LocalVariableLocator::Variables NaiveLocalVariableLocator::findLocalVariablesRequiredForStmts(clang::StmtRange stmts)
{
    DeclRefExprVisitor v;
    v.traverseStmts(stmts);
    return v.getVariables();
}
