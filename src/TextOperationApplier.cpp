#include "TextOperationApplier.hpp"
#include <stdexcept>

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
    auto copy = text;
    for (auto const& ins : insertions)
    {
        unsigned insertionOffset = ins.first;
        auto textToInsert = ins.second;
        copy = copy.substr(0, insertionOffset) + textToInsert + copy.substr(insertionOffset);
    }
    for (auto const& rem : removals)
    {
        auto removal = rem.second;
        copy = copy.substr(0, removal.from) + copy.substr(removal.to);
    }
    return copy;
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    insertions[offset] += text;
}

void TextOperationApplier::removeTextInRange(unsigned int from, unsigned int to) 
{
    Range range{from, to};
    verifyNoOverlappingRangesExist(range);
    removals[from] = range;
}
void TextOperationApplier::verifyNoOverlappingRangesExist(const TextOperationApplier::Range& r)
{
    for (auto const& rem : removals)
        if (rem.second.overlapsWith(r))
            throw std::invalid_argument("TextOperationApplier: overlapping range");
}
