#include "TextOperationApplier.hpp"
#include <stdexcept>
#include <cassert>

TextOperationApplier::Range::Range(unsigned int from, unsigned int to)
    : from(from), to(to)
{
    if (to < from)
        throw std::invalid_argument("TextOperationApplier: invalid range");
}

bool TextOperationApplier::Range::overlapsWith(const TextOperationApplier::Range& r) const
{
    return (from < r.to && r.to <= to) || (from <= r.from && r.from < to);
}

std::string TextOperationApplier::apply(const std::string& text)
{
    auto current = text;
    for (auto const& it : replacements)
    {
        unsigned offset = it.first;
        auto const& op = it.second;
        current = current.substr(0, op.removal.from) + current.substr(op.removal.to);
        current = current.substr(0, offset) + op.insertion + current.substr(offset);
    }
    return current;
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    replacements[offset].insertion += text;
}

void TextOperationApplier::removeTextInRange(unsigned int from, unsigned int to) 
{
    Range range{from, to};
    verifyNoOverlappingRangesExist(range);
    replacements[from].removal = range;
}
void TextOperationApplier::verifyNoOverlappingRangesExist(const TextOperationApplier::Range& r)
{
    for (auto const& rem : replacements)
        if (rem.second.removal.overlapsWith(r))
            throw std::invalid_argument("TextOperationApplier: overlapping range");
}
