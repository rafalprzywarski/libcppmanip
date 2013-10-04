#include "findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

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

class DeclaredVariablesVisitor : public clang::RecursiveASTVisitor<DeclaredVariablesVisitor>
{
public:
    bool VisitVarDecl(clang::VarDecl *d)
    {
        declared.insert(d);
        return true;
    }
    std::unordered_set<clang::VarDecl *> getDeclared() const { return declared; }
private:
    std::unordered_set<clang::VarDecl *> declared;
};

std::vector<clang::VarDecl *> findVariablesUsedByStmts(clang::StmtRange stmts)
{
    UsedVariablesVisitor v;
    for (auto stmt : stmts)
        v.TraverseStmt(stmt);
    return v.getUsed();
}

std::unordered_set<clang::VarDecl *> findVariablesDeclaredByStmts(clang::StmtRange stmts)
{
    DeclaredVariablesVisitor v;
    for (auto stmt : stmts)
        v.TraverseStmt(stmt);
    return v.getDeclared();
}

ast::LocalVariablePtr asLocalVariable(clang::VarDecl *d)
{
    return std::make_shared<ast::LocalVariable>(d->getNameAsString(), d->getType().getAsString() + " " + d->getNameAsString());
}

}

ast::LocalVariables findVariablesDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent)
{
    using namespace boost::adaptors;
    auto declared = findVariablesDeclaredByStmts(stmts);
    auto used = findVariablesUsedByStmts({end(stmts), parent.child_end()});
    ast::LocalVariables result;
    auto isDeclared = [&](clang::VarDecl *d) { return declared.count(d) != 0; };
    boost::push_back(result, used | filtered(isDeclared) | transformed(std::ptr_fun(&asLocalVariable)));
    return result;
}

}
}
