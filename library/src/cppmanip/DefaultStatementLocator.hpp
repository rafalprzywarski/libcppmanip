#ifndef CPPMANIP_DEFAULTSTATEMENTLOCATOR_HPP
#define CPPMANIP_DEFAULTSTATEMENTLOCATOR_HPP
#include "StatementLocator.hpp"
#include "LocationRange.hpp"

namespace cppmanip
{

class DefaultStatementLocator : public StatementLocator
{
public:
    DefaultStatementLocator(LocationRange selection) { }
    virtual clang::StmtRange findStatementsInFunction(const clang::FunctionDecl& decl);
};

}
#endif // CPPMANIP_DEFAULTSTATEMENTLOCATOR_HPP
