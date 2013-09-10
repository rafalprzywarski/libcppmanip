#include "DelayedMethodExtractor.hpp"
#include "SourceExtractor.hpp"
#include "TextOperationApplier.hpp"
#include "FunctionPrinter.hpp"
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_set>

DelayedMethodExtractor::DelayedMethodExtractor(
    SourceExtractor& sourceExtractor, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter, LocalVariableLocator& localVariableLocator)
    : sourceExtractor(sourceExtractor), sourceOperations(sourceOperations), functionPrinter(functionPrinter), localVariableLocator(localVariableLocator) { }

void DelayedMethodExtractor::extractStatmentsFromFunctionIntoNewFunction(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName)
{
    auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
    auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    auto variables = localVariableLocator.findLocalVariablesRequiredForStmts(stmts);
    printExtractedFunction(originalFunctionLocation, extractedFunctionName, variables, stmtsRange);
    replaceStatementsWithFunctionCall(stmtsRange, extractedFunctionName, variables);
}

void DelayedMethodExtractor::printExtractedFunction(
    clang::SourceLocation at, const std::string& name, const DelayedMethodExtractor::Variables& variables, clang::SourceRange stmtsRange)
{
    auto args = getTypesAndNames(variables);
    auto source = functionPrinter.printFunction(name, args, sourceExtractor.getSource(stmtsRange));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void DelayedMethodExtractor::replaceStatementsWithFunctionCall(
    clang::SourceRange stmtsRange, const std::string& functionName, const DelayedMethodExtractor::Variables& variables)
{
    replaceRangeWith(stmtsRange, functionPrinter.printFunctionCall(functionName, getNames(variables)));
}

void DelayedMethodExtractor::replaceRangeWith(clang::SourceRange without, std::string replace)
{
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    sourceOperations.removeTextInRange(begin, end);
    sourceOperations.insertTextAt(replace, begin);
}

FunctionPrinter::Strings DelayedMethodExtractor::getTypesAndNames(DelayedMethodExtractor::Variables variables)
{
    FunctionPrinter::Strings args;
    for (auto d : variables)
        args.push_back(sourceExtractor.getVarDecl(*d));
    return args;
}

FunctionPrinter::Strings DelayedMethodExtractor::getNames(DelayedMethodExtractor::Variables variables)
{
    FunctionPrinter::Strings args;
    for (auto d : variables)
        args.push_back(sourceExtractor.getVarName(*d));
    return args;
}
