#ifndef OFFSETRANGE_HPP
#define OFFSETRANGE_HPP
#include <stdexcept>

/**
 * A left-closed right-open range, with the exception that degenerate ranges overlap non-degenerate ranges.
 */
class OffsetRange
{
public:
    OffsetRange() : from(0), to(0) { }
    OffsetRange(unsigned from, unsigned to)
        : from(from), to(to)
    {
        if (to < from)
            throw std::invalid_argument("invalid range");
    }
    bool degenerate() const { return from == to; }
    unsigned length() const { return to - from; }
    bool overlapsWith(const OffsetRange& right) const
    {
        if (right.from < from)
            return right.overlapsWith(*this);
        return (right.to != from) && (right.from < to);
    }
private:
    unsigned from, to;
};

#endif // OFFSETRANGE_HPP
