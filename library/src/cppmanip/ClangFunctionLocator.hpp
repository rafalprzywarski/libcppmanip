#ifndef CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
#define CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
#include <clang/AST/Decl.h>
#include "OffsetRange.hpp"

namespace cppmanip {

class ClangFunctionLocator
{
public:
    virtual ~ClangFunctionLocator() { }
    virtual clang::FunctionDecl& getFunctionContainingSelection(OffsetRange selection) = 0;
};

}

#endif // CPPMANIP_E7CD9B82765343D98CDD6BB3FB1AF245_HPP
