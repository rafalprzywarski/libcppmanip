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
        : definitionOffset(definitionOffset), statements(std::make_shared<Statements>(statements)) { }
    SourceOffset getDefinitionOffset() const { return definitionOffset; }
    StatementsPtr getStatements() const { return statements; }
private:
    const SourceOffset definitionOffset;
    const StatementsPtr statements;
};

typedef std::shared_ptr<const Function> FunctionPtr;

}
}
#endif // CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
