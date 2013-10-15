#ifndef DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
#define DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
#include "ReplacementFunctionPrinter.hpp"

namespace cppmanip
{
namespace format
{

class DefaultReplacementFunctionPrinter : public ReplacementFunctionPrinter
{
public:
    typedef std::function<ast::LocalVariables(ast::StatementRange stmts)> FindLocalVariablesRequiredForStmts;
    typedef std::function<std::string(ast::StatementRange stmts)> FormatStatements;
    typedef std::function<std::string(const std::string& name, const std::vector<std::string>& args)> PrintFunctionCall;
    typedef std::function<std::string(const std::string& type, const std::string& name, const std::vector<std::string>& args, const std::string& body)> PrintFunctionDefinition;

    DefaultReplacementFunctionPrinter(
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts, FormatStatements formatStatements,
        PrintFunctionCall printFunctionCall, PrintFunctionDefinition printFunctionDefinition)
        : findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts), formatStatements(formatStatements),
        printFunctionCall(printFunctionCall), printFunctionDefinition(printFunctionDefinition) { }
    virtual ReplacementFunction printFunctionFromStmts(const std::string& name, ast::StatementRange stmts);
private:
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FormatStatements formatStatements;
    PrintFunctionCall printFunctionCall;
    PrintFunctionDefinition printFunctionDefinition;
};

}
}
#endif // DEFAULTREPLACEMENTFUNCTIONPRINTER_HPP
