#ifndef CPPMANIP_BA3BF11E9FFD41DCA628C27DFD250E89_HPP
#define CPPMANIP_BA3BF11E9FFD41DCA628C27DFD250E89_HPP
#include "FunctionExtractionValidator.hpp"

namespace cppmanip
{

class NoReturnFunctionExtractionValidator : public FunctionExtractionValidator
{
public:
    typedef std::function<ast::LocalVariables(ast::StatementRange stmts, const ast::Statements& scope)> FindVariablesDeclaredByAndUsedAfterStmts;
    NoReturnFunctionExtractionValidator(FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts)
        : findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts) { }
    virtual void validateStatements(const std::string& functionName, ast::StatementRange selected, ast::FunctionPtr originalFunction);
private:
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
};

}
#endif // CPPMANIP_BA3BF11E9FFD41DCA628C27DFD250E89_HPP
