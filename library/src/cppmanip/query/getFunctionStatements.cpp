#include "getFunctionStatements.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <set>
#include <unordered_set>
#include <unordered_map>

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
        if (var && !isGlobal(var))
            used.insert(var);
        return true;
    }
    std::vector<clang::VarDecl *> get() const { return {used.begin(), used.end()}; }
private:
    std::unordered_set<clang::VarDecl *> used;

    bool isGlobal(clang::VarDecl *d) const
    {
        return d->getParentFunctionOrMethod() == nullptr;
    }

};

typedef std::unordered_map<clang::VarDecl *, ast::LocalVariablePtr> LocalSymbols;

std::vector<clang::VarDecl *> getUsedVarDecls(clang::Stmt& stmt)
{
    UsedVariablesVisitor v;
    v.TraverseStmt(&stmt);
    return v.get();
}

std::vector<clang::VarDecl *> getDeclaredVarDecls(clang::Stmt& stmt)
{
    auto decl = clang::dyn_cast<clang::DeclStmt>(&stmt);
    if (!decl)
        return {};
    std::vector<clang::VarDecl *> vars;
    for (auto d = decl->decl_begin(); d != decl->decl_end(); ++d)
        vars.push_back(clang::dyn_cast<clang::VarDecl>(*d));
    return vars;
}

ast::LocalVariablePtr asLocalVariable(clang::VarDecl *v)
{
    return std::make_shared<ast::LocalVariable>(v->getNameAsString(), v->getType().getAsString() + " " + v->getNameAsString());
}

ast::LocalVariables getDeclaredVars(clang::Stmt& stmt, LocalSymbols& locals)
{
    ast::LocalVariables declared;
    for (auto var : getDeclaredVarDecls(stmt))
    {
        auto local = asLocalVariable(var);
        locals.insert({var, local});
        declared.push_back(local);
    }
    return declared;
}

ast::LocalVariables getUsedLocalVars(clang::Stmt& stmt, const LocalSymbols& locals)
{
    ast::LocalVariables used;
    for (auto var : getUsedVarDecls(stmt))
    {
        auto found = locals.find(var);
        if (found != locals.end())
            used.push_back(found->second);
    }
    return used;
}

template <typename GetStmtRange>
ast::StatementPtr translateStmt(clang::Stmt& stmt, LocalSymbols& locals, GetStmtRange getStmtRange)
{
    auto declared = getDeclaredVars(stmt, locals);
    auto used = getUsedLocalVars(stmt, locals);
    auto range = getStmtRange(stmt);
    return std::make_shared<ast::Statement>(range, declared, used);
}

}

ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange)
{
    ast::Statements stmts;
    LocalSymbols locals;
    for (auto stmt : f.getBody()->children())
        stmts.push_back(
            translateStmt(
                *stmt, locals, [&](clang::Stmt& s) { return getStmtRange(f.getASTContext().getSourceManager(), s); }));
    return stmts;
}

}
}
