#include "DefaultFunctionLocator.hpp"
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
    Visitor(LocationRange selection) : selection(selection), foundDecl() { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!decl->hasBody() || !getBodyRange(decl).overlapsWith(selection))
            return true;
        foundDecl = decl;
        return false;
    }
    clang::FunctionDecl *getFoundDecl() const { return foundDecl; }
private:
    LocationRange selection;
    clang::FunctionDecl* foundDecl;
    LocationRange getBodyRange(clang::FunctionDecl* decl)
    {
        auto& sourceManager = decl->getASTContext().getSourceManager();
        const auto CLOSING_BRACE = 1;
        auto s = decl->getBody()->getLocStart();
        auto e = decl->getBody()->getLocEnd().getLocWithOffset(CLOSING_BRACE);
        return LocationRange(
            rowCol(sourceManager.getSpellingLineNumber(s) - 1, sourceManager.getSpellingColumnNumber(s) - 1),
            rowCol(sourceManager.getSpellingLineNumber(e) - 1, sourceManager.getSpellingColumnNumber(e) - 1));
    }
};

}

clang::FunctionDecl& DefaultFunctionLocator::getFunction(clang::ASTContext& context)
{
    Visitor v(selection);
    v.TraverseDecl(context.getTranslationUnitDecl());
    if (!v.getFoundDecl())
        throw ExtractMethodError("Selection not found");
    return *v.getFoundDecl();
}

}
