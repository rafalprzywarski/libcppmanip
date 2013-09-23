#ifndef CPPMANIP_143F175862E14387A557B8C4624C6193_HPP
#define CPPMANIP_143F175862E14387A557B8C4624C6193_HPP
#include <stdexcept>
#include <ostream>

namespace cppmanip
{

/**
 * A left-closed right-open range, with the exception that degenerate ranges overlap non-degenerate ranges.
 */
template <typename Position>
class PositionRange
{
public:
    PositionRange() : from(), to() { }
    PositionRange(Position from, Position to)
        : from(from), to(to)
    {
        if (to < from)
            throw std::invalid_argument("invalid range");
    }
    bool degenerate() const { return from == to; }
    bool overlapsWith(const PositionRange& right) const
    {
        if (right.from < from)
            return right.overlapsWith(*this);
        return !(right.to == from) && (right.from < to);
    }
    bool operator==(const PositionRange<Position>& right) const
    {
        return from == right.from && to == right.to;
    }
    friend std::ostream& operator<<(std::ostream& os, const PositionRange<Position>& p)
    {
        return os << "[" << p.from << "; " << p.to << ")";
    }
private:
    Position from, to;
};

}
#endif // CPPMANIP_143F175862E14387A557B8C4624C6193_HPP
