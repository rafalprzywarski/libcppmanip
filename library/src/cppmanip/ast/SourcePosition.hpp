#ifndef CPPMANIP_357717E398AA461688871605ED7F697E_HPP
#define CPPMANIP_357717E398AA461688871605ED7F697E_HPP
#include "SourceLocation.hpp"

namespace cppmanip
{
namespace ast
{

typedef unsigned SourceOffset;

class SourcePosition
{
public:
    SourcePosition(SourceOffset offset, SourceLocation location)
        : offset(offset), location(location) { }
    SourceOffset getOffset() const { return offset; }
    SourceLocation getLocation() const { return location; }
private:
    SourceOffset offset;
    SourceLocation location;
};

}
}

#endif // CPPMANIP_357717E398AA461688871605ED7F697E_HPP
