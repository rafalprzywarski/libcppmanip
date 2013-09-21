#include "ParsedFunction.hpp"
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>

namespace cppmanip
{
namespace test
{

namespace
{

class FunctionDeclVisitor : public clang::RecursiveASTVisitor<FunctionDeclVisitor>
{
public:
    FunctionDeclVisitor(clang::StmtRange& range)
        : range(range) { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!decl->hasBody())
            return true;
        range = decl->getBody()->children();
        return false;
    }
private:
    clang::StmtRange& range;
};

class ParseFunctionASTConsumer : public clang::ASTConsumer
{
public:
    ParseFunctionASTConsumer(clang::ASTContext *&astContext, clang::StmtRange& range, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), range(range), parsed(parsed), canFinish(canFinish) { }
    virtual void HandleTranslationUnit(clang::ASTContext& Ctx)
    {
        FunctionDeclVisitor vv(range);
        vv.TraverseDecl(Ctx.getTranslationUnitDecl());
        astContext = &Ctx;
        parsed.notify();
        canFinish.wait();
    }
private:
    clang::ASTContext *&astContext;
    clang::StmtRange& range;
    Notifier& parsed;
    Waiter& canFinish;
};

class ParseFunctionFrontendAction : public clang::ASTFrontendAction
{
public:
    ParseFunctionFrontendAction(clang::ASTContext *&astContext, clang::StmtRange& range, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), range(range), parsed(parsed), canFinish(canFinish) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile)
    {
        return new ParseFunctionASTConsumer(astContext, range, parsed, canFinish);
    }
private:
    clang::ASTContext *&astContext;
    clang::StmtRange& range;
    Notifier& parsed;
    Waiter& canFinish;
};

}

ParsedFunction::ParsedFunction(const std::string& source)
    : thread([&]{ clang::tooling::runToolOnCode(new ParseFunctionFrontendAction(astContext, range, parsed, canFinish), source); })
{
    parsed.wait();
}

ParsedFunction::~ParsedFunction() {
    canFinish.notify();
    thread.join();
}

clang::StmtRange ParsedFunction::stmts()
{
    return range;
}

}
}
