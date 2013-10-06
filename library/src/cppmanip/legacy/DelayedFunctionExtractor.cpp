#include "DelayedFunctionExtractor.hpp"
#include <cppmanip/boundary/ExtractMethodError.hpp>
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

std::vector<std::string> getArgumentDeclarations(const ast::LocalVariables& variables)
{
    std::vector<std::string> args;
    for (auto d : variables)
        args.push_back(d->getNameWithType());
    return args;
}

ast::SourceOffsetRange getRange(ast::StatementRange stmts)
{
    return { stmts.front()->getRange().getFrom(), stmts.back()->getRange().getTo() };
}

std::string getSource(ast::StatementRange stmts)
{
    std::string source;
    for (auto stmt : stmts)
    {
        source += stmt->getSourceCode();
        if (stmt != stmts.back())
            source += stmt->getSourceCodeAfter();
    }
    return source;
}

}

void DelayedFunctionExtractor::extractStatmentsFromFunction(ast::StatementRange stmts, ast::FunctionPtr originalFunction)
{
    failIfVariablesAreDeclaredByAndUsedAfterStmts(stmts, originalFunction);
    auto requiredVars = findLocalVariablesRequiredForStmts(stmts);

    insertFunctionWithArgsAndBody(originalFunction->getDefinitionOffset(), getArgumentDeclarations(requiredVars), getSource(stmts));
    replaceStatementsWithFunctionCall(getRange(stmts), requiredVars);
}

void DelayedFunctionExtractor::insertFunctionWithArgsAndBody(
    ast::SourceOffset at, const std::vector<std::string>& variables, std::string body)
{
    auto function = printFunctionDefinition("void", extractedFunctionName, variables, body);
    sourceOperations.insertTextAt(function, at);
}

void DelayedFunctionExtractor::replaceStatementsWithFunctionCall(
    ast::SourceOffsetRange stmts, const ast::LocalVariables& variables)
{
    replaceRangeWith(stmts.getFrom(), stmts.getTo(), printFunctionCallStmt(extractedFunctionName, variables));
}

void DelayedFunctionExtractor::replaceRangeWith(unsigned from, unsigned to, std::string replacement)
{
    sourceOperations.removeTextInRange(from, to);
    sourceOperations.insertTextAt(replacement, from);
}

std::string DelayedFunctionExtractor::printFunctionCallStmt(const std::string& name, const ast::LocalVariables& args)
{
    using boost::adaptors::transformed;
    std::vector<std::string> argNames;
    boost::push_back(argNames, args | transformed(std::bind(&ast::LocalVariable::getName, std::placeholders::_1)));
    return printFunctionCall(name, argNames) + ";";
}

namespace
{

std::string printOrderedVariableNameList(const ast::LocalVariables& variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::bind(&ast::LocalVariable::getName, std::placeholders::_1)));
    boost::sort(names);
    return boost::algorithm::join(names, ", ");
}

}

void DelayedFunctionExtractor::failIfVariablesAreDeclaredByAndUsedAfterStmts(ast::StatementRange stmts, ast::FunctionPtr originalFunction)
{
    auto usedVars = findVariablesDeclaredByAndUsedAfterStmts(stmts, originalFunction->getStatements());
    if (!usedVars.empty())
        throw boundary::ExtractMethodError("Cannot extract \'" + extractedFunctionName +
            "\'. Following variables are in use after the selected statements: " + printOrderedVariableNameList(usedVars));

}

}
}
