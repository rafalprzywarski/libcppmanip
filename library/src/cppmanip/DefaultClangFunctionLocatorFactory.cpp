#include "DefaultClangFunctionLocatorFactory.hpp"
#include "DefaultClangFunctionLocator.hpp"

namespace cppmanip
{

std::shared_ptr<ClangFunctionLocator> DefaultClangFunctionLocatorFactory::createFunctionLocator(clang::ASTContext& context)
{
    return std::make_shared<DefaultClangFunctionLocator>(context);
}

}

