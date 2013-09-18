#include "OffsetConverter.hpp"
#include <algorithm>
#include <stdexcept>

SourceLocation OffsetConverter::getLocationFromOffset(unsigned int offset) const
{
    validateOffset(offset);
    SourceLocation loc;
    loc.col = calcColumn(offset);
    loc.row = calcRow(offset);
    return loc;
}

void OffsetConverter::validateOffset(unsigned int offset) const
{
    if (offset >= source.length())
        throw std::out_of_range("offset too large");
}

unsigned int OffsetConverter::calcColumn(unsigned int offset) const
{
    auto lastEol = source.rfind('\n', offset);
    if (lastEol == std::string::npos)
        return offset;
    return offset - lastEol - 1;
}

unsigned int OffsetConverter::calcRow(unsigned int offset) const
{
    return std::count(source.begin(), source.begin() + offset, '\n');
}
