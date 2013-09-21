#ifndef CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#define CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#include <clang/AST/StmtIterator.h>
#include <clang/AST/Decl.h>

class MethodExtractor
{
public:
    virtual ~MethodExtractor() { }
    virtual void extractStatmentsFromFunctionIntoNewFunction(
        clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName) = 0;
};

#endif // CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
