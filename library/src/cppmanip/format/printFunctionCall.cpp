#include "printFunctionCall.hpp"
#include <boost/algorithm/string/join.hpp>

namespace cppmanip
{
namespace format
{

std::string printFunctionCall(const std::string& name, const std::vector<std::string>& args)
{
    return name + "(" + boost::algorithm::join(args, ", ") + ")";
}

}
}
