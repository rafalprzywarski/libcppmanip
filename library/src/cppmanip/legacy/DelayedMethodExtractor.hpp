#ifndef CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#define CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
#include "FunctionPrinter.hpp"
#include "LocalVariableLocator.hpp"
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
    DelayedMethodExtractor(
        text::OffsetBasedTextModifier& sourceOperations, FunctionPrinter& functionPrinter, LocalVariableLocator& localVariableLocator, const std::string& extractedFunctionName);

    void extractStatmentsFromFunction(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);

private:

    typedef LocalVariableLocator::Variables Variables;

    text::OffsetBasedTextModifier& sourceOperations;
    FunctionPrinter& functionPrinter;
    LocalVariableLocator& localVariableLocator;
    std::string extractedFunctionName;

    void printExtractedFunction(clang::SourceLocation at, const std::string& name, const Variables& variables, clang::SourceRange stmtsRange, SourceExtractor& sourceExtractor);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName, const Variables& variables, SourceExtractor& sourceExtractor);
    void replaceRangeWith(clang::SourceRange without, std::string replace, SourceExtractor& sourceExtractor);
    FunctionPrinter::Strings getTypesAndNames(Variables variables, SourceExtractor& sourceExtractor);
    FunctionPrinter::Strings getNames(Variables variables, SourceExtractor& sourceExtractor);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName, SourceExtractor& sourceExtractor);
    std::string getVarDecl(const clang::VarDecl& d) const;
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
