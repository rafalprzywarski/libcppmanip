#ifndef CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
#define CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
#include "ClangFunctionLocatorFactory.hpp"

namespace cppmanip
{

class DefaultClangFunctionLocatorFactory : public ClangFunctionLocatorFactory
{
public:
    virtual std::shared_ptr<ClangFunctionLocator> createFunctionLocator(OffsetRange selection);
};

}
#endif // CPPMANIPDEFAULTCLANGFUNCTIONLOCATORFACTORY_HPP
