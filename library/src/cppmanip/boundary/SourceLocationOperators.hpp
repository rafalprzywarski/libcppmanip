#ifndef CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
#define CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
#include <cppmanip/boundary/SourceLocation.hpp>
#include <ostream>

namespace cppmanip
{
namespace boundary
{

inline bool operator==(const SourceLocation& left, const SourceLocation& right)
{
    return left.row == right.row && left.col == right.col;
}

inline bool operator<(const SourceLocation& left, const SourceLocation& right)
{
    if (left.row == right.row)
        return left.col < right.col;
    return left.row < right.row;
}

inline std::ostream& operator<<(std::ostream& os, SourceLocation l)
{
    return os << "(" << l.row << ", " << l.col << ")";
}

}
}
#endif // CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
