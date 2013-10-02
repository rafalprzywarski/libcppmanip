#ifndef CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
#define CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
#include "SourcePosition.hpp"

namespace clang { class FunctionDecl; }

namespace cppmanip
{
namespace ast
{

class Function
{
public:
    Function(clang::FunctionDecl& decl, SourceOffset definitionOffset)
        : decl(&decl), definitionOffset(definitionOffset) { }
    clang::FunctionDecl& getDecl() const { return *decl; }
    SourceOffset getDefinitionOffset() const { return definitionOffset; }
private:
    clang::FunctionDecl *decl; // TODO: temporarily
    SourceOffset definitionOffset;
};

}
}
#endif // CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
