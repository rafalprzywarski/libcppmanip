#ifndef CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
#define CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
#include "ClangFunctionLocatorFactory.hpp"

namespace cppmanip
{

class DefaultClangFunctionLocatorFactory : public ClangFunctionLocatorFactory
{
public:
    virtual std::shared_ptr<ClangFunctionLocator> createFunctionLocator(clang::ASTContext& context);
};

}
#endif // CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
