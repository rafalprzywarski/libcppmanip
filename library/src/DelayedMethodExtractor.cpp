#include "DelayedMethodExtractor.hpp"
#include "SourceExtractor.hpp"
#include "TextOperationApplier.hpp"
#include "FunctionPrinter.hpp"

DelayedMethodExtractor::DelayedMethodExtractor(
    SourceExtractor& sourceExtractor, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter)
    : sourceExtractor(sourceExtractor), sourceOperations(sourceOperations), functionPrinter(functionPrinter) { }

void DelayedMethodExtractor::extractStatmentsFromFunctionIntoNewFunction(
    clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName)
{
    auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
    auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    printExtractedFunction(originalFunctionLocation, extractedFunctionName, stmtsRange);
    replaceStatementsWithFunctionCall(stmtsRange, extractedFunctionName);
}

void DelayedMethodExtractor::printExtractedFunction(
    clang::SourceLocation at, const std::string& name, clang::SourceRange stmtsRange)
{
    auto source = functionPrinter.printFunction(name, sourceExtractor.getSource(stmtsRange));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void DelayedMethodExtractor::replaceStatementsWithFunctionCall(
    clang::SourceRange stmtsRange, const std::string& functionName)
{
    replaceRangeWith(stmtsRange, functionPrinter.printFunctionCall(functionName));
}

void DelayedMethodExtractor::replaceRangeWith(clang::SourceRange without, std::string replace)
{
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    sourceOperations.removeTextInRange(begin, end);
    sourceOperations.insertTextAt(replace, begin);
}
