#include "TextOperationApplier.hpp"
#include <stdexcept>

namespace cppmanip
{

void TextOperationApplier::apply(cppmanip::TextReplacementListener& replacer) const
{
    for (auto const& it : replacements)
        it.second.applyAt(it.first, replacer);
}

void TextOperationApplier::insertTextAt(const std::string& text, Position pos)
{
    verifyNoOverlappingRangesExist(OffsetRange(pos, pos));
    replacements[pos].appendInsertionText(text);
}

void TextOperationApplier::removeTextInRange(Position from, Position to)
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
        if (rep.second.overlapsWithRangeAt(r, rep.first))
            throw std::invalid_argument("TextOperationApplier: overlapping range");
}
void TextOperationApplier::Replacement::applyAt(Position offset, TextReplacementListener& listner) const
{
    listner.replaceWithTextInRange(insertionText, offset, getRemovalEnd(offset));
}

void TextOperationApplier::Replacement::appendInsertionText(const std::string& s)
{
    insertionText += s;
}
void TextOperationApplier::Replacement::setRemovalEnd(Position end)
{
    removalEnd = end;
}
bool TextOperationApplier::Replacement::overlapsWithRangeAt(const OffsetRange& r, Position offset) const
{
    return OffsetRange(offset, getRemovalEnd(offset)).overlapsWith(r);
}
TextOperationApplier::Position TextOperationApplier::Replacement::getRemovalEnd(Position offset) const
{
    return removalEnd.get_value_or(offset);
}

}
