#include "NaiveLocalVariableLocator.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/count.hpp>

namespace cppmanip
{
namespace legacy
{

namespace
{

class VariableFinder
{
public:
    typedef std::vector<clang::VarDecl *> Variables;
    Variables getVariables() const
    {
        return variables;
    }
protected:
    Variables variables;
};

class UsedVariablesFinder : public clang::RecursiveASTVisitor<UsedVariablesFinder>, public VariableFinder
{
public:
    void findAfterStmts(clang::StmtRange stmts, clang::Stmt& parent)
    {
        for (auto& s : clang::StmtRange(end(stmts), end(parent.children())))
            TraverseStmt(s);
    }

    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast<clang::VarDecl>(d->getDecl());
        if (!var || isGlobal(var))
            return true;
        variables.push_back(var);
        return true;
    }
private:

    bool isGlobal(clang::VarDecl *var)
    {
        return var->getParentFunctionOrMethod() == nullptr;
    }
};

class DeclaredLocalVariablesFinder : public clang::RecursiveASTVisitor<DeclaredLocalVariablesFinder>, public VariableFinder
{
public:
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
};

}

NaiveLocalVariableLocator::Variables NaiveLocalVariableLocator::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, clang::Stmt& parent)
{
    DeclaredLocalVariablesFinder declaredFinder;
    declaredFinder.traverseStmts(stmts);
    auto declared = declaredFinder.getVariables();

    UsedVariablesFinder usedFinder;
    usedFinder.findAfterStmts(stmts, parent);
    auto used = usedFinder.getVariables();

    auto isDeclared = [&](clang::VarDecl *d) { return !boost::empty(boost::find<boost::return_found_end>(declared, d)); };

    Variables result;
    boost::push_back(result, used | boost::adaptors::filtered(isDeclared));
    return result;
}

}
}
