#include "getFunctionStatements.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
{

namespace
{

ast::LocalVariables getDeclaredVars(clang::Stmt& stmt)
{
    auto decl = clang::dyn_cast<clang::DeclStmt>(&stmt);
    if (!decl)
        return ast::LocalVariables();
    ast::LocalVariables vars;
    for (auto d = decl->decl_begin(); d != decl->decl_end(); ++d)
    {
        auto v = clang::dyn_cast<clang::VarDecl>(*d);
        vars.push_back(std::make_shared<ast::LocalVariable>(v->getNameAsString(), v->getType().getAsString() + " " + v->getNameAsString()));
    }
    return vars;
}

}

ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange)
{
    ast::Statements stmts;
    for (auto stmt : f.getBody()->children())
        stmts.push_back(std::make_shared<ast::Statement>(
            getStmtRange(f.getASTContext().getSourceManager(), *stmt), getDeclaredVars(*stmt), ast::LocalVariables()));
    return stmts;
}

}
}
