#include "DefaultClangFunctionLocator.hpp"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

namespace
{

class Visitor : public clang::RecursiveASTVisitor<Visitor>
{
public:
    Visitor() : foundDecl() { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        foundDecl = decl;
        return false;
    }
    clang::FunctionDecl *getFoundDecl() const { return foundDecl; }
private:
    clang::FunctionDecl* foundDecl;
};

}

clang::FunctionDecl& DefaultClangFunctionLocator::getFunctionContainingSelection(OffsetRange selection)
{
    Visitor v;
    v.TraverseDecl(context.getTranslationUnitDecl());
    return *v.getFoundDecl();
}

}
