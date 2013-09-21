#ifndef CPPMANIP_5D4C3D0051814BD1A4454E38B8795C0F_HPP
#define CPPMANIP_5D4C3D0051814BD1A4454E38B8795C0F_HPP
#include <clang/AST/Decl.h>
#include <clang/AST/StmtIterator.h>

namespace cppmanip
{

class StatementLocator
{
public:
    virtual ~StatementLocator() { }

    virtual clang::StmtRange findStatementsInFunction(const clang::FunctionDecl& decl) = 0;
};

}
#endif // CPPMANIP_5D4C3D0051814BD1A4454E38B8795C0F_HPP
