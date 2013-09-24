#ifndef CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#define CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#include <string>
#include <map>
#include <stdexcept>
#include <boost/optional.hpp>
#include "TextReplacementListener.hpp"
#include "TextModifier.hpp"
#include <cppmanip/math/PositionRange.hpp>

namespace cppmanip
{
namespace text
{

template <typename Position>
class TextOperationApplier : public TextModifier<Position>
{
public:
    void apply(TextReplacementListener<Position>& replacer) const
    {
        for (auto const& it : replacements)
            it.second.applyAt(it.first, replacer);
    }
    void insertTextAt(const std::string& text, Position pos)
    {
        verifyNoOverlappingRangesExist(OffsetRange(pos, pos));
        replacements[pos].appendInsertionText(text);
    }
    void removeTextInRange(Position from, Position to)
    {
        OffsetRange range{from, to};
        verifyNoOverlappingRangesExist(range);
        if (range.degenerate())
            return;
        replacements[from].setRemovalEnd(to);
    }
private:
    typedef math::PositionRange<Position> OffsetRange;
    class Replacement
    {
    public:
        void appendInsertionText(const std::string& s)
        {
            insertionText += s;
        }
        void setRemovalEnd(Position end)
        {
            removalEnd = end;
        }
        bool overlapsWithRangeAt(const OffsetRange& r, Position pos) const
        {
            return OffsetRange(pos, getRemovalEnd(pos)).overlapsWith(r);
        }
        void applyAt(Position pos, TextReplacementListener<Position>& listner) const
        {
            listner.replaceWithTextInRange(insertionText, pos, getRemovalEnd(pos));
        }
    private:
        boost::optional<Position> removalEnd;
        std::string insertionText;
        Position getRemovalEnd(Position pos) const
        {
            return removalEnd.get_value_or(pos);
        }
    };
    std::map<Position, Replacement, std::greater<Position> > replacements;

    void verifyNoOverlappingRangesExist(const OffsetRange& r)
    {
        for (auto const& rep : replacements)
            if (rep.second.overlapsWithRangeAt(r, rep.first))
                throw std::invalid_argument("TextOperationApplier: overlapping range");
    }
};

typedef TextOperationApplier<unsigned> OffsetBasedTextOperationApplier;

}
}
#endif // CPPMANIP_01077C995F92441DB2604431793CC665_HPP
