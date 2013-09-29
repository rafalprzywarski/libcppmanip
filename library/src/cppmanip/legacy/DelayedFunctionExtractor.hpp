#ifndef CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#define CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#include "SourceExtractor.hpp"
#include <cppmanip/StatementExtractor.hpp>
#include <cppmanip/text/TextModifier.hpp>

namespace cppmanip
{
namespace legacy
{

class DelayedFunctionExtractor : public StatementExtractor
{
public:
    typedef std::vector<clang::VarDecl *> Variables;
    typedef std::function<Variables(clang::StmtRange stmts)> FindLocalVariablesRequiredForStmts;
    typedef std::function<Variables(clang::StmtRange stmts, clang::Stmt& parent)> FindVariablesDeclaredByAndUsedAfterStmts;
    typedef std::function<std::string(const std::string&, const std::vector<std::string>&)> PrintFunctionCall;
    typedef std::function<std::string(const std::string&, const std::string&, const std::vector<std::string>&, const std::string&)> PrintFunctionDefinition;
    typedef std::function<unsigned(clang::SourceLocation)> GetLocationOffset;
    typedef std::function<clang::SourceRange(clang::StmtRange stmts)> GetStmtsRange;
    DelayedFunctionExtractor(
        text::OffsetBasedTextModifier& sourceOperations, PrintFunctionCall printFunctionCall, PrintFunctionDefinition printFunctionDefinition,
        GetLocationOffset getLocationOffset, GetStmtsRange getStmtsRange,
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts,
        FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts, const std::string& extractedFunctionName)
        : sourceOperations(sourceOperations), printFunctionCall(printFunctionCall), printFunctionDefinition(printFunctionDefinition),
        getLocationOffset(getLocationOffset), getStmtsRange(getStmtsRange),
        findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts),
        findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts), extractedFunctionName(extractedFunctionName)
    {
    }

    void extractStatmentsFromFunction(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);

private:

    text::OffsetBasedTextModifier& sourceOperations;
    PrintFunctionCall printFunctionCall;
    PrintFunctionDefinition printFunctionDefinition;
    GetLocationOffset getLocationOffset;
    GetStmtsRange getStmtsRange;
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
    const std::string extractedFunctionName;

    void printExtractedFunction(const clang::FunctionDecl& originalFunction, const Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor);
    void replaceStatementsWithFunctionCall(clang::StmtRange stmts, const Variables& variables, SourceExtractor& sourceExtractor);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string printVariableNames(Variables variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);
    std::string printFunction(const std::string& name, const Variables& vars, const std::string& body);
    std::string printFunctionCallStmt(const std::string& name, const Variables& args);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
