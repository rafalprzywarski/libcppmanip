#include "getASTContextForSource.hpp"
#include "Condition.hpp"
#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>

namespace cppmanip
{
namespace test
{

namespace
{

class ParseASTConsumer : public clang::ASTConsumer
{
public:
    ParseASTConsumer(clang::ASTContext *&astContext, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), parsed(parsed), canFinish(canFinish) { }
    virtual void HandleTranslationUnit(clang::ASTContext& Ctx)
    {
        astContext = &Ctx;
        parsed.notify();
        canFinish.wait();
    }
private:
    clang::ASTContext *&astContext;
    Notifier& parsed;
    Waiter& canFinish;
};

class ParseFrontendAction : public clang::ASTFrontendAction
{
public:
    ParseFrontendAction(clang::ASTContext *&astContext, Notifier& parsed, Waiter& canFinish)
        : astContext(astContext), parsed(parsed), canFinish(canFinish) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance& CI, llvm::StringRef InFile)
    {
        return new ParseASTConsumer(astContext, parsed, canFinish);
    }
private:
    clang::ASTContext *&astContext;
    Notifier& parsed;
    Waiter& canFinish;
};

class StillASTContext : boost::noncopyable
{
public:
    StillASTContext(const std::string& source)
        : sourceForTwine(source),
        thread([&]{
            clang::tooling::runToolOnCode(new ParseFrontendAction(astContext, parsed, canFinish), sourceForTwine);
        })
    {
        parsed.wait();
    }
    ~StillASTContext()
    {
        canFinish.notify();
        thread.join();
    }
    clang::ASTContext *getASTContext() const { return astContext; }
private:
    std::string sourceForTwine;
    Condition parsed, canFinish;
    clang::ASTContext *astContext;
    boost::thread thread;
};

}

std::shared_ptr<clang::ASTContext> getASTContextForSource(const std::string& source)
{
    auto stillContext = std::make_shared<StillASTContext>(source);
    return std::shared_ptr<clang::ASTContext>(stillContext, stillContext->getASTContext());
}

}
}
