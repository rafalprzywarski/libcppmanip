#include "OffsetConverter.hpp"
#include <algorithm>

SourceLocation OffsetConverter::getLocationFromOffset(unsigned int offset) const
{
    SourceLocation loc;
    loc.col = offset;
    auto lastEol = source.rfind('\n', offset);
    if (lastEol != std::string::npos)
        loc.col -= lastEol + 1;
    loc.row = std::count(source.begin(), source.begin() + offset, '\n');
    return loc;
}
