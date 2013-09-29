#ifndef CPPMANIP_5EBDAEC4EC494A1E8F3986ECB16F99DC_HPP
#define CPPMANIP_5EBDAEC4EC494A1E8F3986ECB16F99DC_HPP
#include <vector>
#include <string>

namespace cppmanip
{
namespace format
{

std::string printFunctionCall(const std::string& name, const std::vector<std::string>& args);
std::string printFunctionDefinition(
    const std::string& type, const std::string& name, const std::vector<std::string>& args, const std::string& body);

}
}
#endif // CPPMANIP_5EBDAEC4EC494A1E8F3986ECB16F99DC_HPP
