#include "DefaultClangFunctionLocator.hpp"
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

namespace
{

class Visitor : public clang::RecursiveASTVisitor<Visitor>
{
public:
    Visitor(OffsetRange selection) : selection(selection), foundDecl() { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!decl->hasBody() || !getBodyRange(decl).overlapsWith(selection))
            return true;
        foundDecl = decl;
        return false;
    }
    clang::FunctionDecl *getFoundDecl() const { return foundDecl; }
private:
    OffsetRange selection;
    clang::FunctionDecl* foundDecl;
    OffsetRange getBodyRange(clang::FunctionDecl* decl)
    {
        auto& sourceManager = decl->getASTContext().getSourceManager();
        const auto CLOSING_BRACE = 1;
        auto s = sourceManager.getFileOffset(sourceManager.getSpellingLoc(decl->getBody()->getLocStart()));
        auto e = sourceManager.getFileOffset(sourceManager.getSpellingLoc(decl->getBody()->getLocEnd())) + CLOSING_BRACE;
        return OffsetRange(s, e);
    }
};

}

clang::FunctionDecl& DefaultClangFunctionLocator::getFunctionContainingSelection(OffsetRange selection)
{
    Visitor v(selection);
    v.TraverseDecl(context.getTranslationUnitDecl());
    if (!v.getFoundDecl())
        throw ExtractMethodError("Selection not found");
    return *v.getFoundDecl();
}

}
