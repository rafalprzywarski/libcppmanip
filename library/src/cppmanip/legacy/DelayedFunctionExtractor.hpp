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
    typedef std::vector<clang::VarDecl *> Variables;
    typedef std::function<Variables(clang::StmtRange stmts)> FindLocalVariablesRequiredForStmts;
    typedef std::function<Variables(clang::StmtRange stmts, clang::Stmt& parent)> FindVariablesDeclaredByAndUsedAfterStmts;
    typedef std::function<std::string(const std::string&, const std::vector<std::string>&)> PrintFunctionCall;
    typedef std::function<std::string(const std::string&, const std::string&, const std::vector<std::string>&, const std::string&)> PrintFunctionDefinition;
    typedef std::function<unsigned(clang::SourceLocation)> GetLocationOffset;
    typedef std::function<clang::SourceRange(clang::StmtRange stmts)> GetStmtsRange;
    typedef std::function<clang::SourceLocation(clang::FunctionDecl&)> GetFunctionDefinitionLocation;
    typedef std::function<std::string(clang::SourceRange stmts)> GetSourceFromRange;
    DelayedFunctionExtractor(
        text::OffsetBasedTextModifier& sourceOperations, PrintFunctionCall printFunctionCall, PrintFunctionDefinition printFunctionDefinition,
        GetLocationOffset getLocationOffset, GetStmtsRange getStmtsRange, GetFunctionDefinitionLocation getFunctionDefinitionLocation,
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts,
        FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts, const std::string& extractedFunctionName,
        GetSourceFromRange getStmtsSource)
        : sourceOperations(sourceOperations), printFunctionCall(printFunctionCall), printFunctionDefinition(printFunctionDefinition),
        getLocationOffset(getLocationOffset), getSourceFromRange(getStmtsRange), getFunctionDefinitionLocation(getFunctionDefinitionLocation),
        findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts),
        findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts), extractedFunctionName(extractedFunctionName),
        getStmtsSource(getStmtsSource)
    {
    }

    void extractStatmentsFromFunction(clang::StmtRange stmts, clang::FunctionDecl& originalFunction);

private:

    text::OffsetBasedTextModifier& sourceOperations;
    PrintFunctionCall printFunctionCall;
    PrintFunctionDefinition printFunctionDefinition;
    GetLocationOffset getLocationOffset;
    GetStmtsRange getSourceFromRange;
    GetFunctionDefinitionLocation getFunctionDefinitionLocation;
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
    const std::string extractedFunctionName;
    GetSourceFromRange getStmtsSource;

    void insertFunctionWithArgsAndBody(clang::SourceLocation at, const std::vector<std::string>& variables, std::string body);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmts, const Variables& variables);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string printVariableNames(Variables variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent);
    std::string printFunctionCallStmt(const std::string& name, const Variables& args);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
