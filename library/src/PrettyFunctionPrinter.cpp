#include "PrettyFunctionPrinter.hpp"
#include <sstream>

std::string PrettyFunctionPrinter::printFunction(const std::string& name, const std::string& body)
{
    std::ostringstream os;
    os << "void " << name << "()\n{\n    " << body << "\n}\n";
    return os.str();
}

std::string PrettyFunctionPrinter::printFunctionCall(const std::string& name)
{
    return name + "();";
}
