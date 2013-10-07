#ifndef CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#define CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#include <string>
#include <map>
#include <stdexcept>
#include <boost/optional.hpp>
#include "OperationRecorder.hpp"
#include <cppmanip/math/PositionRange.hpp>

namespace cppmanip
{
namespace text
{

template <typename Position>
class StrictOperationRecorder : public OperationRecorder<Position>
{
public:
    void insertTextAt(const std::string& text, Position pos)
    {
        verifyNoOverlappingRangesExist(PositionRange(pos, pos));
        replacements[pos].appendInsertionText(text);
    }
    void removeTextInRange(Position from, Position to)
    {
        PositionRange range{from, to};
        verifyNoOverlappingRangesExist(range);
        if (range.degenerate())
            return;
        replacements[from].setRemovalEnd(to);
    }
    Replacements<Position> getReplacements() const
    {
        Replacements<Position> result;
        for (auto const& rep : replacements)
            result.push_back(rep.second.getReplacement(rep.first));
        return result;
    }
private:
    typedef math::PositionRange<Position> PositionRange;
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
        bool overlapsWithRangeAt(const PositionRange& r, Position pos) const
        {
            return PositionRange(pos, getRemovalEnd(pos)).overlapsWith(r);
        }
        text::Replacement<Position> getReplacement(Position from) const
        {
            return { from, getRemovalEnd(from), insertionText };
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

    void verifyNoOverlappingRangesExist(const PositionRange& r)
    {
        for (auto const& rep : replacements)
            if (rep.second.overlapsWithRangeAt(r, rep.first))
                throw std::invalid_argument("TextOperationApplier: overlapping range");
    }
};

typedef StrictOperationRecorder<unsigned> OffsetBasedStrictOperationRecorder;

}
}
#endif // CPPMANIP_01077C995F92441DB2604431793CC665_HPP
