#ifndef CPPMANIP_LOCALVARIABLE_HPP
#define CPPMANIP_LOCALVARIABLE_HPP
#include <string>

namespace cppmanip {

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

#endif // CPPMANIP_LOCALVARIABLE_HPP
