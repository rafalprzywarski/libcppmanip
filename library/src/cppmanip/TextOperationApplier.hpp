#ifndef CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#define CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#include <string>
#include <map>
#include <boost/optional.hpp>
#include "TextReplacementListener.hpp"
#include "PositionRange.hpp"

namespace cppmanip
{

class TextOperationApplier
{
public:
    typedef unsigned Position;
    void apply(TextReplacementListener<Position>& replacer) const;
    void insertTextAt(const std::string& text, Position pos);
    void removeTextInRange(Position from, Position to);
private:
    typedef PositionRange<Position> OffsetRange;
    class Replacement
    {
    public:
        void appendInsertionText(const std::string& s);
        void setRemovalEnd(Position end);
        bool overlapsWithRangeAt(const OffsetRange& r, Position pos) const;
        void applyAt(Position pos, TextReplacementListener<Position>& listner) const;
    private:
        boost::optional<Position> removalEnd;
        std::string insertionText;
        Position getRemovalEnd(Position pos) const;
    };
    std::map<Position, Replacement, std::greater<Position> > replacements;

    void verifyNoOverlappingRangesExist(const OffsetRange& r);
};

}
#endif // CPPMANIP_01077C995F92441DB2604431793CC665_HPP
