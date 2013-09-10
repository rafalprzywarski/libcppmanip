#ifndef DELAYEDMETHODEXTRACTOR_HPP
#define DELAYEDMETHODEXTRACTOR_HPP
#include "MethodExtractor.hpp"
#include "FunctionPrinter.hpp"
#include "LocalVariableLocator.hpp"

class TextOperationApplier;
class SourceExtractor;
class DelayedMethodExtractor : public MethodExtractor
{
public:
    DelayedMethodExtractor(
        SourceExtractor& sourceExtractor, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter, LocalVariableLocator& localVariableLocator);

    void extractStatmentsFromFunctionIntoNewFunction(
        clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName);

private:

    typedef LocalVariableLocator::Variables Variables;

    SourceExtractor& sourceExtractor;
    TextOperationApplier& sourceOperations;
    FunctionPrinter& functionPrinter;
    LocalVariableLocator& localVariableLocator;

    void printExtractedFunction(clang::SourceLocation at, const std::string& name, const Variables& variables, clang::SourceRange stmtsRange);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName, const Variables& variables);
    void replaceRangeWith(clang::SourceRange without, std::string replace);
    FunctionPrinter::Strings getTypesAndNames(Variables variables);
    FunctionPrinter::Strings getNames(Variables variables);
};

#endif // DELAYEDMETHODEXTRACTOR_HPP
