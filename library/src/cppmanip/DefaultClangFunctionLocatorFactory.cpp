#include "DefaultClangFunctionLocatorFactory.hpp"
#include "DefaultClangFunctionLocator.hpp"

namespace cppmanip
{

std::shared_ptr<ClangFunctionLocator> DefaultClangFunctionLocatorFactory::createFunctionLocator(OffsetRange selection)
{
    return std::make_shared<DefaultClangFunctionLocator>(selection);
}

}

