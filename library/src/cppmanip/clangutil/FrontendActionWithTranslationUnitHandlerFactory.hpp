#ifndef CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#define CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Tooling.h>
#include "HandleTranslationUnit.hpp"

namespace cppmanip
{
namespace clangutil
{

class ASTConsumerWithTranslationUnitHandler : public clang::ASTConsumer
{
public:
    ASTConsumerWithTranslationUnitHandler(clangutil::HandleTranslationUnit handleTranslationUnit)
        : handleTranslationUnit(handleTranslationUnit) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx)
    {
        handleTranslationUnit(ctx);
    }
private:
    const clangutil::HandleTranslationUnit handleTranslationUnit;
};

class FrontendActionWithTranslationUnitHandler : public clang::ASTFrontendAction
{
public:
    FrontendActionWithTranslationUnitHandler(HandleTranslationUnit handleTranslationUnit)
        : handleTranslationUnit(handleTranslationUnit) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance&, clang::StringRef)
    {
        return new ASTConsumerWithTranslationUnitHandler(handleTranslationUnit);
    }
private:
    const HandleTranslationUnit handleTranslationUnit;
};

class FrontendActionWithTranslationUnitHandlerFactory : public clang::tooling::FrontendActionFactory
{
public:
    FrontendActionWithTranslationUnitHandlerFactory(HandleTranslationUnit handleTranslationUnit)
        : handleTranslationUnit(handleTranslationUnit) { }

    virtual clang::FrontendAction* create()
    {
        return new FrontendActionWithTranslationUnitHandler(handleTranslationUnit);
    }

private:
    const HandleTranslationUnit handleTranslationUnit;
};

}
}
#endif // CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
