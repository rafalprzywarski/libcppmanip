#ifndef CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#define CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#include "ClangFunctionLocator.hpp"
#include "LocationRange.hpp"

namespace cppmanip
{

class DefaultClangFunctionLocator : public ClangFunctionLocator
{
public:
    DefaultClangFunctionLocator(LocationRange selection) : selection(selection) { }
    virtual clang::FunctionDecl& getFunction(clang::ASTContext& context);
private:
    LocationRange selection;
};

}
#endif // CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
