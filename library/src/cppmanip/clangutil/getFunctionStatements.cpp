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
namespace clangutil
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

class StatementTranslator
{
public:
    StatementTranslator(GetStatementRange getStmtRange, clang::SourceManager& sourceManager)
        : getStmtRange(getStmtRange), sourceManager(sourceManager) { }

    ast::StatementsPtr translateStmts(clang::StmtRange range)
    {
        ast::Statements stmts;
        for (auto stmt = range; stmt; ++stmt)
        {
            auto nextStmt = boost::next(stmt);
            stmts.emplace_back(translateStmt(**stmt, !nextStmt.empty() ? *nextStmt : nullptr));
        }
        return std::make_shared<ast::Statements>(std::move(stmts));
    }

private:
    typedef std::unordered_map<clang::VarDecl *, ast::LocalVariablePtr> LocalSymbols;

    GetStatementRange getStmtRange;
    clang::SourceManager& sourceManager;
    LocalSymbols locals;

    ast::StatementPtr translateStmt(clang::Stmt& stmt, clang::Stmt *nextStmt)
    {
        auto declared = getDeclaredVars(stmt);
        auto used = getUsedLocalVars(stmt);
        auto range = getStmtRange(sourceManager, stmt);
        auto specific = getSpecificRanges(stmt);
        auto sourceCode = getSourceCode(range);
        auto nextFrom = nextStmt ? getStmtRange(sourceManager, *nextStmt).getFrom() : range.getTo();
        auto sourceCodeAfter = getSourceCode({ range.getTo(), nextFrom });
        auto children = getChildren(stmt);
        return std::make_shared<ast::Statement>(range, specific, declared, used, sourceCode, sourceCodeAfter, children);
    }

    ast::StatementGroups getChildren(clang::Stmt& stmt)
    {
        auto tryStmt = clang::dyn_cast<clang::CXXTryStmt>(&stmt);
        if (!tryStmt)
            return {};
        ast::StatementGroups groups;
        groups.emplace_back(translateStmts(tryStmt->getTryBlock()->children()));
        for (unsigned i = 0; i < tryStmt->getNumHandlers(); ++i)
            groups.emplace_back(translateStmts(tryStmt->getHandler(i)->getHandlerBlock()->children()));
        return groups;
    }

    std::string getSourceCode(ast::SourceOffsetRange range)
    {
        auto loc = sourceManager.getLocForStartOfFile(sourceManager.getMainFileID()).getLocWithOffset(range.getFrom()); // TODO: this is ugly!
        return std::string(sourceManager.getCharacterData(loc), range.getTo() - range.getFrom());
    }

    ast::LocalVariables getUsedLocalVars(clang::Stmt& stmt)
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

    ast::LocalVariables getDeclaredVars(clang::Stmt& stmt)
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

    ast::LocalVariablePtr asLocalVariable(clang::VarDecl *v)
    {
        return std::make_shared<ast::LocalVariable>(
            v->getNameAsString(), v->getType().getAsString() + " " + v->getNameAsString(),
            sourceManager.getFileOffset(v->getLocation()));
    }

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
    ast::SourceOffsetRanges getSpecificRanges(clang::Stmt& stmt)
    {
        auto tryStmt = clang::dyn_cast<clang::CXXTryStmt>(&stmt);
        if (!tryStmt)
            return {};
        return {{sourceManager.getFileOffset(tryStmt->getTryLoc()), sourceManager.getFileOffset(tryStmt->getTryBlock()->getLBracLoc()) + 1}};
    }
};

}

ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange)
{
    StatementTranslator translator(getStmtRange, f.getASTContext().getSourceManager());
    return *translator.translateStmts(f.getBody()->children());
}

}
}
