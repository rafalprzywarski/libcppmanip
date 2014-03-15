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
    FileBasedStatementLocator(
        const std::string& filename, ast::GatewayPtr astGateway, GetOffsetFromLocation getOffsetFromLocation,
        FindSelectedStatementsInFunction findSelectedStatementsInFunction)
        : filename(filename), astGateway(astGateway), getOffsetFromLocation(getOffsetFromLocation),
        findSelectedStatementsInFunction(findSelectedStatementsInFunction) { }
    FunctionAndStmts getSelectedFunctionAndStmts(boundary::SourceSelection selection);
private:
    std::string filename;
    ast::GatewayPtr astGateway;
    GetOffsetFromLocation getOffsetFromLocation;
    FindSelectedStatementsInFunction findSelectedStatementsInFunction;
};

}

#endif // CPPMANIP_29B9D10E4A854ABA8889178431D516F8_HPP
