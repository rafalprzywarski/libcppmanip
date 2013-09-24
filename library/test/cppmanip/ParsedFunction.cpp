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
    FunctionDeclVisitor(clang::FunctionDecl *& function)
        : function(function) { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!decl->hasBody())
            return true;
        function = decl;
        return false;
    }
private:
    clang::FunctionDecl *& function;
};

class ParseFunctionASTConsumer : public clang::ASTConsumer
{
public:
    ParseFunctionASTConsumer(clang::ASTContext *&astContext, clang::FunctionDecl *& function, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), function(function), parsed(parsed), canFinish(canFinish) { }
    virtual void HandleTranslationUnit(clang::ASTContext& Ctx)
    {
        parse(Ctx);
        parsed.notify();
        canFinish.wait();
    }
private:
    clang::ASTContext *&astContext;
    clang::FunctionDecl *& function;
    Notifier& parsed;
    Waiter& canFinish;
    void parse(clang::ASTContext& ctx)
    {
        FunctionDeclVisitor vv(function);
        vv.TraverseDecl(ctx.getTranslationUnitDecl());
        astContext = &ctx;
    }
};

class ParseFunctionFrontendAction : public clang::ASTFrontendAction
{
public:
    ParseFunctionFrontendAction(clang::ASTContext *&astContext, clang::FunctionDecl *& function, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), function(function), parsed(parsed), canFinish(canFinish) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile)
    {
        return new ParseFunctionASTConsumer(astContext, function, parsed, canFinish);
    }
private:
    clang::ASTContext *&astContext;
    clang::FunctionDecl *& function;
    Notifier& parsed;
    Waiter& canFinish;
};

}

ParsedFunction::ParsedFunction(const std::string& source)
    : sourceForTwine(source),
    thread([&]{ clang::tooling::runToolOnCode(new ParseFunctionFrontendAction(astContext, function, parsed, canFinish), sourceForTwine); })
{
    parsed.wait();
}

ParsedFunction::~ParsedFunction() {
    canFinish.notify();
    thread.join();
}

clang::FunctionDecl* ParsedFunction::getDecl()
{
    return function;
}

clang::StmtRange ParsedFunction::stmts()
{
    return function->getBody()->children();
}

}
}
