#ifndef CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#define CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
#include <clang/AST/StmtIterator.h>
#include <clang/AST/Decl.h>
#include "ast/Function.hpp"

namespace cppmanip
{

class StatementExtractor
{
public:
    virtual ~StatementExtractor() { }
    virtual void extractStatmentsFromFunction(
        clang::StmtRange stmts, ast::FunctionPtr originalFunction) = 0;
};

typedef std::shared_ptr<StatementExtractor> StatementExtractorPtr;
}
#endif // CPPMANIP_106D220E33A2477EA1C1472F2F1F1325_HPP
