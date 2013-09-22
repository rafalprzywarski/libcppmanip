#ifndef CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
#define CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
#include <clang/AST/RecursiveASTVisitor.h>
#include "StatementLocator.hpp"

namespace cppmanip
{

class MethodExtractorVisitor : public clang::RecursiveASTVisitor<MethodExtractorVisitor>
{
public:
    MethodExtractorVisitor(StatementLocator& stmtLocator)
        : stmtLocator(stmtLocator) { }

    bool VisitFunctionDecl(clang::FunctionDecl* decl);

    clang::FunctionDecl *getFunctionDecl() const { return decl; }

private:
    clang::FunctionDecl* decl;
    StatementLocator& stmtLocator;
};

}
#endif // CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
