#include "SourceLocationConverter.hpp"
#include <stdexcept>

namespace cppmanip
{

unsigned int SourceLocationConverter::getOffsetFromLocation(SourceLocation loc) const
{
    return loc.col + calcRowOffset(loc.row);
}

unsigned int SourceLocationConverter::calcRowOffset(unsigned int row) const
{
    std::string::const_iterator it = source.begin();
    while (row > 0)
    {
        if (it == source.end())
            throw std::out_of_range("row too large");
        if (*it == '\n')
            row--;
        it++;
    }
    return (it - source.begin());
}

}
