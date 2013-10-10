#ifndef CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
#define CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
#include <string>
#include <vector>
#include <memory>
#include "SourceOffset.hpp"

namespace cppmanip
{
namespace ast
{

class LocalVariable
{
public:
    LocalVariable(const std::string& name, const std::string& nameWithType, SourceOffset declarationOffset)
        : name(name), nameWithType(nameWithType), declarationOffset(declarationOffset) { }
    const std::string& getName() const { return name; }
    const std::string& getNameWithType() const { return nameWithType; }
    SourceOffset getDeclarationOffset() const { return declarationOffset; }
private:
    std::string name;
    std::string nameWithType;
    SourceOffset declarationOffset;
};

typedef std::shared_ptr<const LocalVariable> LocalVariablePtr;
typedef std::vector<LocalVariablePtr> LocalVariables;

}
}
#endif // CPPMANIP_B40BEC39700E4028957DA15BE006B348_HPP
