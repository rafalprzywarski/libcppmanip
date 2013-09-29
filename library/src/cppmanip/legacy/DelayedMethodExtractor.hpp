#ifndef CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#define CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#include "FunctionPrinter.hpp"
#include "SourceExtractor.hpp"
#include <cppmanip/StatementExtractor.hpp>
#include <cppmanip/text/TextModifier.hpp>

namespace cppmanip
{
namespace legacy
{

class DelayedMethodExtractor : public StatementExtractor
{
public:
    typedef std::vector<clang::VarDecl *> Variables;
    typedef std::function<Variables(clang::StmtRange stmts)> FindLocalVariablesRequiredForStmts;
    typedef std::function<Variables(clang::StmtRange stmts, clang::Stmt& parent)> FindVariablesDeclaredByAndUsedAfterStmts;
    DelayedMethodExtractor(
        text::OffsetBasedTextModifier& sourceOperations, FunctionPrinter& functionPrinter,
        FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts,
        FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts, const std::string& extractedFunctionName)
        : sourceOperations(sourceOperations), functionPrinter(functionPrinter), findLocalVariablesRequiredForStmts(findLocalVariablesRequiredForStmts),
        findVariablesDeclaredByAndUsedAfterStmts(findVariablesDeclaredByAndUsedAfterStmts), extractedFunctionName(extractedFunctionName)
    {
    }

    void extractStatmentsFromFunction(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);

private:

    text::OffsetBasedTextModifier& sourceOperations;
    FunctionPrinter& functionPrinter;
    FindLocalVariablesRequiredForStmts findLocalVariablesRequiredForStmts;
    FindVariablesDeclaredByAndUsedAfterStmts findVariablesDeclaredByAndUsedAfterStmts;
    const std::string extractedFunctionName;

    void printExtractedFunction(const clang::FunctionDecl& originalFunction, const Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor);
    void replaceStatementsWithFunctionCall(clang::StmtRange stmts, const Variables& variables, SourceExtractor& sourceExtractor);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string getNames(Variables variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
