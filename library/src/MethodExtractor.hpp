#ifndef METHODEXTRACTOR_HPP
#define METHODEXTRACTOR_HPP
#include <clang/AST/StmtIterator.h>
#include <clang/AST/Decl.h>

class MethodExtractor
{
public:
    virtual ~MethodExtractor() { }
    virtual void extractStatmentsFromFunctionIntoNewFunction(
        clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName) = 0;
};

#endif // METHODEXTRACTOR_HPP
