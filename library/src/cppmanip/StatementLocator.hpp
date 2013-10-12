#ifndef CPPMANIP_175DE2760C484E24900AB69193574E9E_HPP
#define CPPMANIP_175DE2760C484E24900AB69193574E9E_HPP
#include <cppmanip/boundary/SourceSelection.hpp>
#include "ast/Function.hpp"

namespace cppmanip
{

class StatementLocator
{
public:
    struct FunctionAndStmts
    {
        ast::FunctionPtr function;
        ast::ScopedStatementRange stmts;
    };

    virtual ~StatementLocator() { }
    virtual FunctionAndStmts getSelectedFunctionAndStmts(boundary::SourceSelection selection) = 0;
};

typedef std::shared_ptr<StatementLocator> StatementLocatorPtr;

}

#endif // CPPMANIP_175DE2760C484E24900AB69193574E9E_HPP
