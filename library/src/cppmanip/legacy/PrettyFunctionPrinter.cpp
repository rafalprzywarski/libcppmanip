#include "PrettyFunctionPrinter.hpp"
#include <cppmanip/format/printFunction.hpp>
#include <sstream>
#include <clang/AST/Decl.h>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

namespace cppmanip
{
namespace legacy
{

std::string PrettyFunctionPrinter::printFunction(const std::string& name, const Variables& vars, const std::string& body)
{
    auto args = getTypesAndNames(vars);
    return format::printFunctionDefinition("void", name, args, body);
}

FunctionPrinter::Strings PrettyFunctionPrinter::getTypesAndNames(Variables variables)
{
    FunctionPrinter::Strings args;
    for (auto d : variables)
        args.push_back(d->getType().getAsString() + " " + d->getNameAsString());
    return args;
}

std::string PrettyFunctionPrinter::printFunctionCall(const std::string& name, const Variables& args)
{
    using boost::adaptors::transformed;
    std::vector<std::string> argNames;
    boost::push_back(argNames, args | transformed(std::mem_fun(&clang::VarDecl::getNameAsString)));
    return format::printFunctionCall(name, argNames) + ";";
}

}
}
