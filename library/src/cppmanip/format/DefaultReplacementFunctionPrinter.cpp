#include "DefaultReplacementFunctionPrinter.hpp"
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{
namespace format
{
namespace
{

std::vector<std::string> getVariableNames(const ast::LocalVariables& variables)
{
    using boost::adaptors::transformed;
    std::vector<std::string> names;
    boost::push_back(names, variables | transformed(std::bind(&ast::LocalVariable::getName, std::placeholders::_1)));
    return names;
}

std::vector<std::string> getArgumentDeclarations(const ast::LocalVariables& variables)
{
    std::vector<std::string> args;
    for (auto d : variables)
        args.push_back(d->getNameWithType());
    return args;
}

}

ReplacementFunction DefaultReplacementFunctionPrinter::printFunctionFromStmts(const std::string& name, ast::StatementRange stmts)
{
    auto required = findLocalVariablesRequiredForStmts(stmts); // TODO: this printer does two steps: generation and printing. Will be split when more features come.
    return {
        printFunctionDefinition("void", name, getArgumentDeclarations(required), formatStatements(stmts)),
        printFunctionCall(name, getVariableNames(required)) };
}

}
}

