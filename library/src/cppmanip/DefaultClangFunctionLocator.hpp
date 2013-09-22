#ifndef CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#define CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#include <clang/AST/Decl.h>
#include "OffsetRange.hpp"

namespace cppmanip
{

class DefaultClangFunctionLocator
{
public:
    DefaultClangFunctionLocator(clang::ASTContext& context) : context(context) { }
    virtual clang::FunctionDecl& getFunctionContainingSelection(OffsetRange selection);
private:
    clang::ASTContext& context;
};

}
#endif // CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
