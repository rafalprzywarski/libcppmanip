#include "getFunctionFromAstInSelection.hpp"
#include <cppmanip/math/PositionRange.hpp>
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
    Visitor(math::PositionRange<ast::SourceOffset> selection) : selection(selection), foundDecl() { }
    bool VisitFunctionDecl(clang::FunctionDecl* decl)
    {
        if (!decl->hasBody() || !getBodyRange(decl).overlapsWith(selection))
            return true;
        foundDecl = decl;
        return false;
    }
    clang::FunctionDecl *getFoundDecl() const { return foundDecl; }
private:
    math::PositionRange<ast::SourceOffset> selection;
    clang::FunctionDecl* foundDecl;
    math::PositionRange<ast::SourceOffset> getBodyRange(clang::FunctionDecl* decl)
    {
        auto& sourceManager = decl->getASTContext().getSourceManager();
        const auto CLOSING_BRACE = 1;
        auto s = decl->getBody()->getLocStart();
        auto e = decl->getBody()->getLocEnd().getLocWithOffset(CLOSING_BRACE);
        return { sourceManager.getFileOffset(s), sourceManager.getFileOffset(e) };
    }
};

ast::SourceOffset getFunctionOffset(clang::ASTContext& context, clang::FunctionDecl *func)
{
    return context.getSourceManager().getFileOffset(func->getLocStart());
}

}

ast::FunctionPtr getFunctionFromAstInSelection(clang::ASTContext& context, ast::SourceOffsetRange selection, GetFunctionStatements getFunctionStatements)
{
    Visitor v({ selection.getFrom(), selection.getTo() });
    v.TraverseDecl(context.getTranslationUnitDecl());
    if (!v.getFoundDecl())
        throw boundary::ExtractMethodError("Selection not found");
    auto func = v.getFoundDecl();
    return std::make_shared<ast::Function>(func, getFunctionOffset(context, func), getFunctionStatements(*func));
}

}
}
