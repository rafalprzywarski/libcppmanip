#include "getFunctionStatements.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
{
ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange)
{
    ast::Statements stmts;
    for (auto stmt : f.getBody()->children())
        stmts.push_back(std::make_shared<ast::Statement>(getStmtRange(f.getASTContext().getSourceManager(), *stmt), ast::LocalVariables(), ast::LocalVariables()));
    return stmts;
}

}
}
