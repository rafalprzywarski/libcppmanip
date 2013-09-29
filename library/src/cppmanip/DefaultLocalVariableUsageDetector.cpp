#include "DefaultLocalVariableUsageDetector.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{

namespace
{

class RequiredVariablesVisitor : public clang::RecursiveASTVisitor<RequiredVariablesVisitor>
{
public:
    bool VisitDeclRefExpr(clang::DeclRefExpr *d)
    {
        auto var = clang::dyn_cast<clang::VarDecl>(d->getDecl());
        if (var && !isGlobal(var))
            used.insert(var);
        return true;
    }

    bool VisitVarDecl(clang::VarDecl *d)
    {
        declared.insert(d);
        return true;
    }

    std::vector<clang::VarDecl *> getRequired() const
    {
        std::vector<clang::VarDecl *> required;
        auto notDeclared = [&](clang::VarDecl *d) { return declared.count(d) == 0; };
        boost::push_back(required, used | boost::adaptors::filtered(notDeclared));
        return required;
    }
private:
    std::unordered_set<clang::VarDecl *> used;
    std::unordered_set<clang::VarDecl *> declared;

    bool isGlobal(clang::VarDecl *d) const
    {
        return d->getParentFunctionOrMethod() == nullptr;
    }
};

}

std::vector<clang::VarDecl *> DefaultLocalVariableUsageDetector::findLocalVariablesRequiredForStmts(
    clang::StmtRange stmts)
{
    RequiredVariablesVisitor v;
    for (auto s : stmts)
        v.TraverseStmt(s);
    return v.getRequired();
}
std::vector<clang::VarDecl *> DefaultLocalVariableUsageDetector::findVariablesDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, clang::Stmt& parent)
{
    return std::vector<clang::VarDecl *>();
}

}

