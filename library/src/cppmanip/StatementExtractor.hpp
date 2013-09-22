#ifndef CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#define CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#include <clang/AST/StmtIterator.h>
#include <clang/AST/Decl.h>

namespace cppmanip
{

class StatementExtractor
{
public:
    virtual ~StatementExtractor() { }
    virtual void extractStatmentsFromFunction(
        clang::StmtRange stmts, const clang::FunctionDecl& originalFunction) = 0;
};

}
#endif // CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
