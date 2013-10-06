#ifndef CPPMANIP_9DCB109EF1064760828DE63EE74C3314_HPP
#define CPPMANIP_9DCB109EF1064760828DE63EE74C3314_HPP
#include <clang/AST/StmtIterator.h>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace clangutil
{
namespace test
{

class ParsedFunction
{
public:
    ParsedFunction(const std::string& source);
    clang::FunctionDecl *getDecl();
    clang::StmtRange stmts();
    clang::ASTContext& getASTContext() { return *astContext; }
    clang::SourceManager& getSourceManager() { return astContext->getSourceManager(); }
private:
    std::shared_ptr<clang::ASTContext> astContext;
};

}
}
}
#endif // CPPMANIP_9DCB109EF1064760828DE63EE74C3314_HPP
