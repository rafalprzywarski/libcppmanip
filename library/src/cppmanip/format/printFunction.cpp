#include "printFunction.hpp"
#include <boost/algorithm/string/join.hpp>

namespace cppmanip
{
namespace format
{

std::string printFunctionCall(const std::string& name, const std::vector<std::string>& args)
{
    return name + "(" + boost::algorithm::join(args, ", ") + ")";
}

std::string printFunctionDefinition(
    const std::string& type, const std::string& name, const std::vector<std::string>& args, const std::string& body)
{
    return type + " " + name + "(" + boost::algorithm::join(args, ", ") + ")\n{\n    " + body + "\n}\n";
}

}
}
