#include "DelayedFunctionExtractor.hpp"
#include "SourceExtractor.hpp"
#include "FunctionPrinter.hpp"
#include <cppmanip/ExtractMethodError.hpp>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <unordered_set>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace cppmanip
{
namespace legacy
{

void DelayedFunctionExtractor::extractStatmentsFromFunction(clang::StmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, originalFunction);
    auto requiredVars = findLocalVariablesRequiredForStmts(stmts);

    SourceExtractor sourceExtractor(originalFunction.getASTContext().getSourceManager());
    printExtractedFunction(originalFunction, requiredVars, stmts, sourceExtractor);
    replaceStatementsWithFunctionCall(stmts, requiredVars, sourceExtractor);
}

void DelayedFunctionExtractor::printExtractedFunction(
    const clang::FunctionDecl& originalFunction, const DelayedFunctionExtractor::Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor)
{
    auto at = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    auto source = functionPrinter.printFunction(extractedFunctionName, variables, sourceExtractor.getSource(stmts));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void DelayedFunctionExtractor::replaceStatementsWithFunctionCall(
    clang::StmtRange stmts, const DelayedFunctionExtractor::Variables& variables, SourceExtractor& sourceExtractor)
{
    auto without = sourceExtractor.getCorrectSourceRange(stmts);
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    replaceRangeWith(begin, end, functionPrinter.printFunctionCall(extractedFunctionName, variables));
}

void DelayedFunctionExtractor::replaceRangeWith(unsigned from, unsigned to, std::string replacement)
{
    sourceOperations.removeTextInRange(from, to);
    sourceOperations.insertTextAt(replacement, from);
}

std::string DelayedFunctionExtractor::getNames(Variables variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::mem_fun(&clang::VarDecl::getNameAsString)));
    boost::sort(names);
    return boost::algorithm::join(names, ", ");
}

void DelayedFunctionExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    auto usedVars = findVariablesDeclaredByAndUsedAfterStmts(stmts, *originalFunction.getBody());
    if (!usedVars.empty())
        throw ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + getNames(usedVars));

}

}
}
