#ifndef CPPMANIP_5CB6327273DE4DAE8C4D35AA5F928281_HPP
#define CPPMANIP_5CB6327273DE4DAE8C4D35AA5F928281_HPP
#include <memory>
#include "ClangFunctionLocator.hpp"

namespace cppmanip {

class ClangFunctionLocatorFactory
{
public:
    virtual ~ClangFunctionLocatorFactory() { }
    virtual std::shared_ptr<ClangFunctionLocator> createFunctionLocator(OffsetRange selection) = 0;
};

}
#endif // CPPMANIP_5CB6327273DE4DAE8C4D35AA5F928281_HPP
