#ifndef STATEMENTLOCATOR_HPP
#define STATEMENTLOCATOR_HPP
#include <clang/AST/Decl.h>
#include <clang/AST/StmtIterator.h>

class StatementLocator
{
public:
    virtual ~StatementLocator() { }

    virtual clang::StmtRange findStatementsInFunction(const clang::FunctionDecl& decl) = 0;
};

#endif // STATEMENTLOCATOR_HPP
