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

    void printExtractedFunction(const clang::FunctionDecl& originalFunction, const Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor);
    void replaceStatementsWithFunctionCall(clang::StmtRange stmts, const Variables& variables, SourceExtractor& sourceExtractor);
    void replaceRangeWith(unsigned int from, unsigned int to, std::string replacement);
    std::string getNames(Variables variables);
    void failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction);
};

}
}
#endif // CPPMANIP_799523E01B0442A6ABBA2BFB7EF4F97A_HPP
