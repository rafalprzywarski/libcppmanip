#include "DelayedFunctionExtractor.hpp"
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

namespace
{

std::vector<std::string> getArgumentDeclarations(const std::vector<clang::VarDecl *>& variables)
{
    std::vector<std::string> args;
    for (auto d : variables)
        args.push_back(d->getType().getAsString() + " " + d->getNameAsString());
    return args;
}

}

void DelayedFunctionExtractor::extractStatmentsFromFunction(clang::StmtRange stmts, clang::FunctionDecl& originalFunction)
{
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, *originalFunction.getBody());
    auto requiredVars = findLocalVariablesRequiredForStmts(stmts);

    insertFunctionWithArgsAndBody(getFunctionDefinitionLocation(originalFunction), getArgumentDeclarations(requiredVars), getStmtsSource(getSourceFromRange(stmts)));
    replaceStatementsWithFunctionCall(getSourceFromRange(stmts), requiredVars);
}

void DelayedFunctionExtractor::insertFunctionWithArgsAndBody(
    clang::SourceLocation at, const std::vector<std::string>& variables, std::string body)
{
    auto function = printFunctionDefinition("void", extractedFunctionName, variables, body);
    sourceOperations.insertTextAt(function, getLocationOffset(at));
}

void DelayedFunctionExtractor::replaceStatementsWithFunctionCall(
    clang::SourceRange stmts, const DelayedFunctionExtractor::Variables& variables)
{
    auto begin = getLocationOffset(stmts.getBegin());
    auto end = getLocationOffset(stmts.getEnd());
    replaceRangeWith(begin, end, printFunctionCallStmt(extractedFunctionName, variables));
}

void DelayedFunctionExtractor::replaceRangeWith(unsigned from, unsigned to, std::string replacement)
{
    sourceOperations.removeTextInRange(from, to);
    sourceOperations.insertTextAt(replacement, from);
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

void DelayedFunctionExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(clang::StmtRange stmts, clang::Stmt& parent)
{
    auto usedVars = findVariablesDeclaredByAndUsedAfterStmts(stmts, parent);
    if (!usedVars.empty())
        throw ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + printOrderedVariableNameList(usedVars));

}

}
}
