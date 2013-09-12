#include "NaiveLocalVariableLocator.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/find.hpp>

namespace
{

class RequiredLocalVariablesFinder : public clang::RecursiveASTVisitor<RequiredLocalVariablesFinder>
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

class UsedVariablesFinder : public clang::RecursiveASTVisitor<UsedVariablesFinder>
{
public:
    typedef std::vector<clang::VarDecl *> Variables;

    Variables getVariables() const
    {
        return variables;
    }
    void findAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& func)
    {
        for (auto& s : clang::StmtRange(end(stmts), func.getBody()->child_end()))
            TraverseStmt(s);
    }

    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast_or_null<clang::VarDecl>(d->getDecl());
        if (!var || isGlobal(var))
            return true;
        variables.push_back(var);
        return true;
    }
private:
    Variables variables;

    bool isGlobal(clang::VarDecl *var)
    {
        return var->getParentFunctionOrMethod() == nullptr;
    }
};

class DeclaredLocalVariablesFinder : public clang::RecursiveASTVisitor<RequiredLocalVariablesFinder>
{
public:
    typedef std::vector<clang::VarDecl *> Variables;

    bool VisitVarDecl(clang::VarDecl *d)
    {
        variables.push_back(d);
        return true;
    }

    virtual void traverseStmts(clang::StmtRange stmts)
    {
        for (auto& s : stmts)
            TraverseStmt(s);
    }

    Variables getVariables() const
    {
        return variables;
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
    RequiredLocalVariablesFinder v;
    v.traverseStmts(stmts);
    return v.getVariables();
}

LocalVariableLocator::Variables NaiveLocalVariableLocator::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, const clang::FunctionDecl& func)
{
    DeclaredLocalVariablesFinder declaredFinder;
    declaredFinder.traverseStmts(stmts);
    auto declared = declaredFinder.getVariables();

    UsedVariablesFinder usedFinder;
    usedFinder.findAfterStmts(stmts, func);
    auto used = usedFinder.getVariables();

    auto isDeclared = [&](clang::VarDecl *d) { return !boost::empty(boost::find<boost::return_found_end>(declared, d)); };

    Variables result;
    boost::push_back(result, used | boost::adaptors::filtered(isDeclared));
    return result;
}
