#ifndef CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
#define CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
#include "SourceOffset.hpp"
#include "Statement.hpp"
#include <memory>

namespace cppmanip
{
namespace ast
{

class Function
{
public:
    Function(const Function& ) = delete;
    Function(SourceOffset definitionOffset, const Statements& statements)
        : definitionOffset(definitionOffset), statements(statements) { }
    SourceOffset getDefinitionOffset() const { return definitionOffset; }
    const Statements& getStatements() const { return statements; }
private:
    const SourceOffset definitionOffset;
    const Statements statements;
};

typedef std::shared_ptr<const Function> FunctionPtr;

}
}
#endif // CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
