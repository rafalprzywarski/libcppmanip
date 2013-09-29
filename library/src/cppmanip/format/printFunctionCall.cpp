#include "printFunctionCall.hpp"

namespace cppmanip
{
namespace format
{

std::string printFunctionCall(const std::string& name, const std::vector<std::string>& args)
{
    return name + "()";
}

}
}
