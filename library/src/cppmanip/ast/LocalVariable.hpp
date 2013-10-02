#ifndef CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
#define CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
#include <string>

namespace cppmanip
{
namespace ast
{

class LocalVariable
{
public:
    LocalVariable(const std::string& name, const std::string& nameWithType)
        : name(name), nameWithType(nameWithType) { }
    const std::string& getName() const { return name; }
    const std::string& getNameWithType() const { return nameWithType; }
private:
    std::string name;
    std::string nameWithType;
};

}
}
#endif // CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
