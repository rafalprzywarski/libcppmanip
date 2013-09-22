#ifndef CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#define CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#include "ClangFunctionLocator.hpp"

namespace cppmanip
{

class DefaultClangFunctionLocator : public ClangFunctionLocator
{
public:
    DefaultClangFunctionLocator(OffsetRange selection) : selection(selection) { }
    virtual clang::FunctionDecl& getFunction(clang::ASTContext& context);
private:
    OffsetRange selection;
};

}
#endif // CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
