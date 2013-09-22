#include "DelayedMethodExtractor.hpp"
#include "SourceExtractor.hpp"
#include "FunctionPrinter.hpp"
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <unordered_set>
#include <boost/algorithm/string/join.hpp>

namespace cppmanip
{

DelayedMethodExtractor::DelayedMethodExtractor(OffsetBasedTextModifier& sourceOperations, FunctionPrinter& functionPrinter,
    LocalVariableLocator& localVariableLocator)
    : sourceOperations(sourceOperations), functionPrinter(functionPrinter), localVariableLocator(localVariableLocator) { }

void DelayedMethodExtractor::extractStatmentsFromFunctionIntoNewFunction(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName)
{
    SourceExtractor sourceExtractor(originalFunction.getASTContext().getSourceManager());
    auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, originalFunction, extractedFunctionName, sourceExtractor);
    auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    auto requiredVars = localVariableLocator.findLocalVariablesRequiredForStmts(stmts);
    printExtractedFunction(originalFunctionLocation, extractedFunctionName, requiredVars, stmtsRange, sourceExtractor);
    replaceStatementsWithFunctionCall(stmtsRange, extractedFunctionName, requiredVars, sourceExtractor);
}

void DelayedMethodExtractor::printExtractedFunction(
    clang::SourceLocation at, const std::string& name, const DelayedMethodExtractor::Variables& variables, clang::SourceRange stmtsRange, SourceExtractor& sourceExtractor)
{
    auto args = getTypesAndNames(variables, sourceExtractor);
    auto source = functionPrinter.printFunction(name, args, sourceExtractor.getSource(stmtsRange));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void DelayedMethodExtractor::replaceStatementsWithFunctionCall(
    clang::SourceRange stmtsRange, const std::string& functionName, const DelayedMethodExtractor::Variables& variables, SourceExtractor& sourceExtractor)
{
    replaceRangeWith(stmtsRange, functionPrinter.printFunctionCall(functionName, getNames(variables, sourceExtractor)), sourceExtractor);
}

void DelayedMethodExtractor::replaceRangeWith(clang::SourceRange without, std::string replace, SourceExtractor& sourceExtractor)
{
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    sourceOperations.removeTextInRange(begin, end);
    sourceOperations.insertTextAt(replace, begin);
}

FunctionPrinter::Strings DelayedMethodExtractor::getTypesAndNames(DelayedMethodExtractor::Variables variables, SourceExtractor& sourceExtractor)
{
    FunctionPrinter::Strings args;
    for (auto d : variables)
        args.push_back(sourceExtractor.getVarDecl(*d));
    return args;
}

FunctionPrinter::Strings DelayedMethodExtractor::getNames(DelayedMethodExtractor::Variables variables, SourceExtractor& sourceExtractor)
{
    FunctionPrinter::Strings args;
    for (auto d : variables)
        args.push_back(sourceExtractor.getVarName(*d));
    return args;
}
void DelayedMethodExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction, const std::string& extractedFunctionName, SourceExtractor& sourceExtractor)
{
    auto usedVars = localVariableLocator.findVariablesDeclaredByAndUsedAfterStmts(stmts, originalFunction);
    if (!usedVars.empty())
        throw ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + boost::algorithm::join(getNames(usedVars, sourceExtractor), ", "));

}

}
