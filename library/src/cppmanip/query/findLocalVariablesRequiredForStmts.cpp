#include "findLocalVariablesRequiredForStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptor/filtered.hpp>
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

    std::vector<clang::VarDecl *> getRequired() const
    {
        std::vector<clang::VarDecl *> required;
        auto notDeclared = [&](clang::VarDecl *d) { return declared.count(d) == 0; };
        boost::push_back(required, used | boost::adaptors::filtered(notDeclared));
        boost::sort(required, [](clang::VarDecl *left, clang::VarDecl *right) { return left->getLocation() < right->getLocation(); });
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

std::vector<clang::VarDecl *> findLocalVariablesRequiredForStmts(
    clang::StmtRange stmts)
{
    RequiredVariablesVisitor v;
    for (auto s : stmts)
        v.TraverseStmt(s);
    return v.getRequired();
}

}
}
