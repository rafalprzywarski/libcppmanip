#ifndef CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
#define CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP

namespace clang { class FunctionDecl; }

namespace cppmanip
{
namespace ast
{

class Function
{
public:
    Function(clang::FunctionDecl& decl) : decl(&decl) { }
    clang::FunctionDecl& getDecl() const { return *decl; }
private:
    clang::FunctionDecl *decl; // TODO: temporarily
};

}
}
#endif // CPPMANIP_257EF455F9F84388866F82E1F5D31143_HPP
