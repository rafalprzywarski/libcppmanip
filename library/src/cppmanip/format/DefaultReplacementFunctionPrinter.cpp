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

ReplacementFunction DefaultReplacementFunctionPrinter::printFunctionFromStmts(const std::string& name, ast::StatementRange stmts)
{
    auto required = findLocalVariablesRequiredForStmts(stmts);
    return {
        printFunctionDefinition("void", name, getArgumentDeclarations(required), getSource(stmts)),
        printFunctionCall(name, getVariableNames(required)) };
}

}
}

