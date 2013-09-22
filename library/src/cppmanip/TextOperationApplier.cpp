#include "TextOperationApplier.hpp"
#include "OffsetRange.hpp"
#include <stdexcept>

namespace cppmanip
{

void TextOperationApplier::apply(cppmanip::TextReplacementListener& replacer) const
{
    for (auto const& it : replacements)
        it.second.applyAtOffset(it.first, replacer);
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    verifyNoOverlappingRangesExist(OffsetRange(offset, offset));
    replacements[offset].appendInsertionText(text);
}

void TextOperationApplier::removeTextInRange(unsigned int from, unsigned int to) 
{
    OffsetRange range{from, to};
    verifyNoOverlappingRangesExist(range);
    if (range.degenerate())
        return;
    replacements[from].setRemovalEnd(to);
}
void TextOperationApplier::verifyNoOverlappingRangesExist(const OffsetRange& r)
{
    for (auto const& rep : replacements)
        if (rep.second.overlapsWithRangeAtOffset(r, rep.first))
            throw std::invalid_argument("TextOperationApplier: overlapping range");
}
void TextOperationApplier::Replacement::applyAtOffset(unsigned int offset, TextReplacementListener& listner) const
{
    listner.replaceWithTextInRange(insertionText, offset, getRemovalEnd(offset));
}

void TextOperationApplier::Replacement::appendInsertionText(const std::string& s)
{
    insertionText += s;
}
void TextOperationApplier::Replacement::setRemovalEnd(unsigned int end)
{
    removalEnd = end;
}
bool TextOperationApplier::Replacement::overlapsWithRangeAtOffset(const OffsetRange& r, unsigned int offset) const
{
    return OffsetRange(offset, getRemovalEnd(offset)).overlapsWith(r);
}
unsigned int TextOperationApplier::Replacement::getRemovalEnd(unsigned int offset) const
{
    return removalEnd.get_value_or(offset);
}

}
