#ifndef CPPMANIP_29B9D10E4A854ABA8889178431D516F8_HPP
#define CPPMANIP_29B9D10E4A854ABA8889178431D516F8_HPP
#include "StatementLocator.hpp"
#include "query/IsStatementSelected.hpp"
#include "ast/Gateway.hpp"

namespace cppmanip {

class FileBasedStatementLocator : public StatementLocator
{
public:
    typedef std::function<ast::SourceOffset(boundary::SourceLocation)> GetOffsetFromLocation;
    typedef std::function<ast::ScopedStatementRange(const ast::Function&, query::IsStatementSelected)> FindSelectedStatementsInFunction;
    typedef std::function<query::StatementSelected(ast::StatementPtr, ast::SourceOffsetRange)> IsStatementSelected;
    FileBasedStatementLocator(
        const std::string& filename, ast::GatewayPtr astGateway, GetOffsetFromLocation getOffsetFromLocation,
        FindSelectedStatementsInFunction findSelectedStatementsInFunction, IsStatementSelected isStatementSelected)
        : filename(filename), astGateway(astGateway), getOffsetFromLocation(getOffsetFromLocation),
        findSelectedStatementsInFunction(findSelectedStatementsInFunction), isStatementSelected(isStatementSelected) { }
    FunctionAndStmts getSelectedFunctionAndStmts(boundary::SourceSelection selection);
private:
    std::string filename;
    ast::GatewayPtr astGateway;
    GetOffsetFromLocation getOffsetFromLocation;
    FindSelectedStatementsInFunction findSelectedStatementsInFunction;
    IsStatementSelected isStatementSelected;
};

}

#endif // CPPMANIP_29B9D10E4A854ABA8889178431D516F8_HPP
