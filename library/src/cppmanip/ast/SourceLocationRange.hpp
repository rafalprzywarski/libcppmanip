#ifndef CPPMANIP_6E5C1D2A22C6457A87B3FA36E02B3B0D_HPP
#define CPPMANIP_6E5C1D2A22C6457A87B3FA36E02B3B0D_HPP
#include "SourceLocation.hpp"

namespace cppmanip
{
namespace ast
{

class SourceLocationRange
{
public:
    SourceLocationRange(SourceLocation from, SourceLocation to)
        : from(from), to(to) { }
    SourceLocation getFrom() const { return from; }
    SourceLocation getTo() const { return to; }
private:
    SourceLocation from, to;
};

inline bool operator==(const SourceLocationRange& left, const SourceLocationRange& right)
{
    return left.getFrom() == right.getFrom() && left.getTo() == right.getTo();
}

inline std::ostream& operator<<(std::ostream& os, const SourceLocationRange& r)
{
    return os << "[" << r.getFrom() << "; " << r.getTo() << ")";
}

}
}
#endif // CPPMANIP_6E5C1D2A22C6457A87B3FA36E02B3B0D_HPP
