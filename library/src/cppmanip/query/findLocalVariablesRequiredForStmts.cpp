#include "findLocalVariablesRequiredForStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{
namespace query
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

    std::vector<LocalVariable> getRequired() const
    {
        using namespace boost::adaptors;
        std::vector<LocalVariable> required;
        auto notDeclared = [&](clang::VarDecl *d) { return declared.count(d) == 0; };
        std::vector<clang::VarDecl *> usedOrdered(used.begin(), used.end());
        boost::sort(usedOrdered, [](clang::VarDecl *left, clang::VarDecl *right) { return left->getLocation() < right->getLocation(); });
        boost::push_back(required, usedOrdered | filtered(notDeclared) | transformed(std::ptr_fun(&asLocalVariable)));
        return required;
    }
private:
    std::unordered_set<clang::VarDecl *> used;
    std::unordered_set<clang::VarDecl *> declared;

    bool isGlobal(clang::VarDecl *d) const
    {
        return d->getParentFunctionOrMethod() == nullptr;
    }

    static LocalVariable asLocalVariable(clang::VarDecl *d)
    {
        return LocalVariable(d->getNameAsString(), d->getType().getAsString() + " " + d->getNameAsString());
    }
};

}

std::vector<LocalVariable> findLocalVariablesRequiredForStmts(
    clang::StmtRange stmts)
{
    RequiredVariablesVisitor v;
    for (auto s : stmts)
        v.TraverseStmt(s);
    return v.getRequired();
}

}
}
