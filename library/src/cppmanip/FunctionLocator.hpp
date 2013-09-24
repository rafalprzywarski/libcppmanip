#ifndef CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
#define CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
#include <clang/AST/Decl.h>

namespace cppmanip {

class FunctionLocator
{
public:
    virtual ~FunctionLocator() { }
    virtual clang::FunctionDecl& getFunction(clang::ASTContext& context) = 0;
};

}

#endif // CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
