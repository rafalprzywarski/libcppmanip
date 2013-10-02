#include "getFunctionFromAstInSelection.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
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
            ast::rowCol(sourceManager.getSpellingLineNumber(s) - 1, sourceManager.getSpellingColumnNumber(s) - 1),
            ast::rowCol(sourceManager.getSpellingLineNumber(e) - 1, sourceManager.getSpellingColumnNumber(e) - 1));
    }
};

}

clang::FunctionDecl& getFunctionFromAstInSelection(clang::ASTContext& context, LocationRange selection)
{
    Visitor v(selection);
    v.TraverseDecl(context.getTranslationUnitDecl());
    if (!v.getFoundDecl())
        throw boundary::ExtractMethodError("Selection not found");
    return *v.getFoundDecl();
}

}
}
