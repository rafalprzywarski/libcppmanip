#ifndef CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
#define CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
#include <cppmanip/SourceLocation.hpp>

namespace cppmanip
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

}
#endif // CPPMANIP_EE24E457CC0A4AC18FC06940E80416B9_HPP
