#ifndef CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#define CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#include <cppmanip/StatementExtractor.hpp>
#include <cppmanip/text/TextModifier.hpp>
#include <cppmanip/ast/LocalVariable.hpp>
#include <clang/Basic/SourceLocation.h>
#include <clang/AST/StmtIterator.h>

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
    typedef std::function<unsigned(clang::SourceLocation)> GetLocationOffset;
    typedef std::function<clang::SourceRange(ast::StatementRange stmts)> GetStmtsRange;
    typedef std::function<std::string(clang::SourceRange stmts)> GetSourceFromRange;
    DelayedFunctionExtractor(
        text::OffsetBasedTextModifier& sourceOperations, PrintFunctionCall printFunctionCall, PrintFunctionDefinition printFunctionDefinition,
        GetLocationOffset getLocationOffset, GetStmtsRange getStmtsRange,
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts,
        FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts, const std::string& extractedFunctionName,
        GetSourceFromRange getStmtsSource)
        : sourceOperations(sourceOperations), printFunctionCall(printFunctionCall), printFunctionDefinition(printFunctionDefinition),
        getLocationOffset(getLocationOffset), getSourceFromRange(getStmtsRange),
        findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts),
        findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts), extractedFunctionName(extractedFunctionName),
        getStmtsSource(getStmtsSource)
    {
    }

    void extractStatmentsFromFunction(ast::StatementRange stmts, ast::FunctionPtr originalFunction);

private:

    text::OffsetBasedTextModifier& sourceOperations;
    PrintFunctionCall printFunctionCall;
    PrintFunctionDefinition printFunctionDefinition;
    GetLocationOffset getLocationOffset;
    GetStmtsRange getSourceFromRange;
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
    const std::string extractedFunctionName;
    GetSourceFromRange getStmtsSource;

    void insertFunctionWithArgsAndBody(ast::SourceOffset at, const std::vector<std::string>& variables, std::string body);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmts, const ast::LocalVariables& variables);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string printVariableNames(const ast::LocalVariables& variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(ast::StatementRange stmts, ast::FunctionPtr originalFunction);
    std::string printFunctionCallStmt(const std::string& name, const ast::LocalVariables& args);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
