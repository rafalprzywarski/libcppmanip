#include "DelayedFunctionExtractor.hpp"
#include "SourceExtractor.hpp"
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

void DelayedFunctionExtractor::extractStatmentsFromFunction(clang::StmtRange stmts, clang::FunctionDecl& originalFunction)
{
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, originalFunction);
    auto requiredVars = findLocalVariablesRequiredForStmts(stmts);

    SourceExtractor sourceExtractor(originalFunction.getASTContext().getSourceManager());
    printExtractedFunction(originalFunction, requiredVars, stmts, sourceExtractor);
    replaceStatementsWithFunctionCall(stmts, requiredVars);
}

void DelayedFunctionExtractor::printExtractedFunction(
    clang::FunctionDecl& originalFunction, const DelayedFunctionExtractor::Variables& variables, clang::StmtRange stmts, SourceExtractor& sourceExtractor)
{
    auto at = getLocationOffset(getFunctionDefinitionLocation(originalFunction));
    auto source = printFunction(extractedFunctionName, variables, sourceExtractor.getSource(getStmtsRange(stmts)));
    sourceOperations.insertTextAt(source, at);
}

void DelayedFunctionExtractor::replaceStatementsWithFunctionCall(
    clang::StmtRange stmts, const DelayedFunctionExtractor::Variables& variables)
{
    auto without = getStmtsRange(stmts);
    auto begin = getLocationOffset(without.getBegin());
    auto end = getLocationOffset(without.getEnd());
    replaceRangeWith(begin, end, printFunctionCallStmt(extractedFunctionName, variables));
}

void DelayedFunctionExtractor::replaceRangeWith(unsigned from, unsigned to, std::string replacement)
{
    sourceOperations.removeTextInRange(from, to);
    sourceOperations.insertTextAt(replacement, from);
}

namespace
{

std::vector<std::string> getTypesAndNames(const std::vector<clang::VarDecl *>& variables)
{
    std::vector<std::string> args;
    for (auto d : variables)
        args.push_back(d->getType().getAsString() + " " + d->getNameAsString());
    return args;
}

}

std::string DelayedFunctionExtractor::printFunction(const std::string& name, const Variables& vars, const std::string& body)
{
    auto args = getTypesAndNames(vars);
    return printFunctionDefinition("void", name, args, body);
}

std::string DelayedFunctionExtractor::printFunctionCallStmt(const std::string& name, const Variables& args)
{
    using boost::adaptors::transformed;
    std::vector<std::string> argNames;
    boost::push_back(argNames, args | transformed(std::mem_fun(&clang::VarDecl::getNameAsString)));
    return printFunctionCall(name, argNames) + ";";
}

namespace
{

std::string printOrderedVariableNameList(std::vector<clang::VarDecl *> variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::mem_fun(&clang::VarDecl::getNameAsString)));
    boost::sort(names);
    return boost::algorithm::join(names, ", ");
}

}

void DelayedFunctionExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(
    clang::StmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    auto usedVars = findVariablesDeclaredByAndUsedAfterStmts(stmts, *originalFunction.getBody());
    if (!usedVars.empty())
        throw ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + printOrderedVariableNameList(usedVars));

}

}
}
