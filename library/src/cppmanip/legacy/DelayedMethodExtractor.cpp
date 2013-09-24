#include "DelayedMethodExtractor.hpp"
#include "SourceExtractor.hpp"
#include "FunctionPrinter.hpp"
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <unordered_set>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace cppmanip
{
namespace legacy
{

DelayedMethodExtractor::DelayedMethodExtractor(text::OffsetBasedTextModifier& sourceOperations, FunctionPrinter& functionPrinter,
    LocalVariableUsageDetector& localVariableLocator, const std::string& extractedFunctionName)
    : sourceOperations(sourceOperations), functionPrinter(functionPrinter), localVariableLocator(localVariableLocator), extractedFunctionName(extractedFunctionName) { }

void DelayedMethodExtractor::extractStatmentsFromFunction(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, originalFunction);
    auto requiredVars = localVariableLocator.findLocalVariablesRequiredForStmts(stmts);

    SourceExtractor sourceExtractor(originalFunction.getASTContext().getSourceManager());
    printExtractedFunction(originalFunction, requiredVars, stmts, sourceExtractor);
    replaceStatementsWithFunctionCall(stmts, requiredVars, sourceExtractor);
}

void DelayedMethodExtractor::printExtractedFunction(
    const clang::FunctionDecl& originalFunction, const DelayedMethodExtractor::Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor)
{
    auto at = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    auto source = functionPrinter.printFunction(extractedFunctionName, variables, sourceExtractor.getSource(stmts));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void DelayedMethodExtractor::replaceStatementsWithFunctionCall(
    clang::StmtRange stmts, const DelayedMethodExtractor::Variables& variables, SourceExtractor& sourceExtractor)
{
    auto without = sourceExtractor.getCorrectSourceRange(stmts);
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    replaceRangeWith(begin, end, functionPrinter.printFunctionCall(extractedFunctionName, variables));
}

void DelayedMethodExtractor::replaceRangeWith(unsigned from, unsigned to, std::string replacement)
{
    sourceOperations.removeTextInRange(from, to);
    sourceOperations.insertTextAt(replacement, from);
}

std::string DelayedMethodExtractor::getNames(Variables variables)
{
    using boost::adaptors::transformed;
    return boost::algorithm::join(variables | transformed(std::mem_fun(&clang::VarDecl::getNameAsString)), ", ");
}

void DelayedMethodExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    auto usedVars = localVariableLocator.findVariablesDeclaredByAndUsedAfterStmts(stmts, *originalFunction.getBody());
    if (!usedVars.empty())
        throw ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + getNames(usedVars));

}

}
}
