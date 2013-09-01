#include "TextOperationApplier.hpp"
#include "OffsetRange.hpp"
#include <stdexcept>

std::string TextOperationApplier::apply(const std::string& text)
{
    auto current = text;
    for (auto const& it : replacements)
        current = it.second.applyAtOffset(it.first, current);
    return current;
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    replacements[offset].appendInsertionText(text);
}

void TextOperationApplier::removeTextInRange(unsigned int from, unsigned int to) 
{
    OffsetRange range{from, to};
    verifyNoOverlappingRangesExist(range);
    if (range.degenerate())
        return;
    replacements[from].setRemovalLength(range.length());
}
void TextOperationApplier::verifyNoOverlappingRangesExist(const OffsetRange& r)
{
    for (auto const& rep : replacements)
        if (rep.second.overlapsWithRangeAtOffset(r, rep.first))
            throw std::invalid_argument("TextOperationApplier: overlapping range");
}
std::string TextOperationApplier::Replacement::applyAtOffset(unsigned int offset, const std::string& s) const
{
    return s.substr(0, offset) + insertionText + s.substr(offset + removalLength);
}
void TextOperationApplier::Replacement::appendInsertionText(const std::string& s)
{
    insertionText += s;
}
void TextOperationApplier::Replacement::setRemovalLength(unsigned int len)
{
    removalLength = len;
}
bool TextOperationApplier::Replacement::overlapsWithRangeAtOffset(const OffsetRange& r, unsigned int offset) const
{
    return OffsetRange(offset, offset + removalLength).overlapsWith(r);
}
