#include "PrettyFunctionPrinter.hpp"
#include <sstream>

namespace cppmanip
{

std::string PrettyFunctionPrinter::printFunction(const std::string& name, const Strings& args, const std::string& body)
{
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

std::string PrettyFunctionPrinter::printFunctionCall(const std::string& name, const FunctionPrinter::Strings& args)
{
    std::ostringstream os;
    os <<  name << "(";
    for (decltype(args.size()) i = 0; i < args.size(); ++i)
    {
        if (i > 0) os << ", ";
        os << args[i];
    }
    os << ");";
    return os.str();
}

}
