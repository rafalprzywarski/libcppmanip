#include "PrettyFunctionPrinter.hpp"
#include <sstream>
#include <clang/AST/Decl.h>

namespace cppmanip
{
namespace legacy
{

std::string PrettyFunctionPrinter::printFunction(const std::string& name, const Variables& vars, const std::string& body)
{
    auto args = getTypesAndNames(vars);
    std::ostringstream os;
    os << "void " << name << "(";
    for (decltype(args.size()) i = 0; i < args.size(); ++i)
    {
        if (i > 0) os << ", ";
        os << args[i];
    }
    os << ")\n{\n    " << body << "\n}\n";
    return os.str();
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
    std::ostringstream os;
    os <<  name << "(";
    for (decltype(args.size()) i = 0; i < args.size(); ++i)
    {
        if (i > 0) os << ", ";
        os << args[i]->getNameAsString();
    }
    os << ");";
    return os.str();
}

}
}
