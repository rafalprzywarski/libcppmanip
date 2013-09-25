#ifndef CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#define CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include "TranslationUnitHandler.hpp"

namespace cppmanip
{
namespace clangutil
{

class ASTConsumerWithTranslationUnitHandler : public clang::ASTConsumer
{
public:
    ASTConsumerWithTranslationUnitHandler(TranslationUnitHandlerPtr translationUnitHandler)
        : translationUnitHandler(translationUnitHandler) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx)
    {
        translationUnitHandler->handleTranslationUnit(ctx);
    }
private:
    const TranslationUnitHandlerPtr translationUnitHandler;
};

class FrontendActionWithTranslationUnitHandler : public clang::ASTFrontendAction
{
public:
    FrontendActionWithTranslationUnitHandler(TranslationUnitHandlerPtr translationUnitHandler)
        : translationUnitHandler(translationUnitHandler) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance&, clang::StringRef)
    {
        return new ASTConsumerWithTranslationUnitHandler(translationUnitHandler);
    }
private:
    const TranslationUnitHandlerPtr translationUnitHandler;
};

class FrontendActionWithTranslationUnitHandlerFactory : public clang::tooling::FrontendActionFactory
{
public:
    FrontendActionWithTranslationUnitHandlerFactory(TranslationUnitHandlerPtr translationUnitHandler)
        : translationUnitHandler(translationUnitHandler) { }

    virtual clang::FrontendAction* create()
    {
        return new FrontendActionWithTranslationUnitHandler(translationUnitHandler);
    }

private:
    const TranslationUnitHandlerPtr translationUnitHandler;
};

}
}
#endif // CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
