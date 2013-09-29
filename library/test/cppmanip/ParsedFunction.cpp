#include "ParsedFunction.hpp"
#include "getASTContextForSource.hpp"
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

}

ParsedFunction::ParsedFunction(const std::string& source)
    : astContext(getASTContextForSource(source))
{
}

clang::FunctionDecl* ParsedFunction::getDecl()
{
    clang::FunctionDecl *function = nullptr;
    FunctionDeclVisitor(function).TraverseDecl(astContext->getTranslationUnitDecl());
    return function;
}

clang::StmtRange ParsedFunction::stmts()
{
    return getDecl()->getBody()->children();
}

}
}
