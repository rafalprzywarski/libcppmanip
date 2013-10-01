#ifndef CPPMANIP_E29FF6C16ED04B95AD82D332F428810A_HPP
#define CPPMANIP_E29FF6C16ED04B95AD82D332F428810A_HPP
#include <stdexcept>

namespace cppmanip
{
namespace boundary
{

class ExtractMethodError : public std::runtime_error
{
public:
    explicit ExtractMethodError(const std::string& msg) : std::runtime_error(msg) { }
};

}
}
#endif // CPPMANIP_E29FF6C16ED04B95AD82D332F428810A_HPP
