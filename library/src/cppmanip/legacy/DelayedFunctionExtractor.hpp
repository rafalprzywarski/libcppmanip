#ifndef CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#define CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#include <cppmanip/StatementExtractor.hpp>
#include <cppmanip/text/TextModifier.hpp>

namespace cppmanip
{
namespace legacy
{

class DelayedFunctionExtractor : public StatementExtractor
{
public:
    typedef std::function<ast::LocalVariables(ast::StatementRange stmts)> FindLocalVariablesRequiredForStmts;
    typedef std::function<ast::LocalVariables(ast::StatementRange stmts, const ast::Statements& scope)> FindVariablesDeclaredByAndUsedAfterStmts;
    typedef std::function<std::string(const std::string&, const std::vector<std::string>&)> PrintFunctionCall;
    typedef std::function<std::string(const std::string&, const std::string&, const std::vector<std::string>&, const std::string&)> PrintFunctionDefinition;
    DelayedFunctionExtractor(
        text::OffsetBasedTextModifier& sourceOperations, PrintFunctionCall printFunctionCall, PrintFunctionDefinition printFunctionDefinition,
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts,
        FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts, const std::string& extractedFunctionName)
        : sourceOperations(sourceOperations), printFunctionCall(printFunctionCall), printFunctionDefinition(printFunctionDefinition),
        findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts),
        findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts), extractedFunctionName(extractedFunctionName)
    {
    }

    void extractStatmentsFromFunction(ast::StatementRange stmts, ast::FunctionPtr originalFunction);

private:

    text::OffsetBasedTextModifier& sourceOperations;
    PrintFunctionCall printFunctionCall;
    PrintFunctionDefinition printFunctionDefinition;
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
    const std::string extractedFunctionName;

    void insertFunctionWithArgsAndBody(ast::SourceOffset at, const std::vector<std::string>& variables, std::string body);
    void replaceStatementsWithFunctionCall(ast::SourceOffsetRange stmts, const ast::LocalVariables& variables);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string printVariableNames(const ast::LocalVariables& variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(ast::StatementRange stmts, ast::FunctionPtr originalFunction);
    std::string printFunctionCallStmt(const std::string& name, const ast::LocalVariables& args);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
