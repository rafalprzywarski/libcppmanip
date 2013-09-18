#include "OffsetConverter.hpp"

SourceLocation OffsetConverter::getLocationFromOffset(unsigned int offset) const
{
    SourceLocation loc;
    loc.col = offset;
    auto lastEol = source.rfind('\n', offset);
    if (lastEol != std::string::npos)
        loc.col -= lastEol + 1;
    return loc;
}
