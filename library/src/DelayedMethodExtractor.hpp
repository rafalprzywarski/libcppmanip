#ifndef DELAYEDMETHODEXTRACTOR_HPP
#define DELAYEDMETHODEXTRACTOR_HPP
#include "MethodExtractor.hpp"

class FunctionPrinter;
class TextOperationApplier;
class SourceExtractor;
class DelayedMethodExtractor : public MethodExtractor
{
public:
    DelayedMethodExtractor(
        SourceExtractor& sourceExtractor, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter);

    void extractStatmentsFromFunctionIntoNewFunction(
        clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName);

private:
    SourceExtractor& sourceExtractor;
    TextOperationApplier& sourceOperations;
    FunctionPrinter& functionPrinter;

    void printExtractedFunction(clang::SourceLocation at, const std::string& name, clang::SourceRange stmtsRange);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName);
    void replaceRangeWith(clang::SourceRange without, std::string replace);
};

#endif // DELAYEDMETHODEXTRACTOR_HPP
