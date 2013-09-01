#ifndef OFFSETRANGE_HPP
#define OFFSETRANGE_HPP

/**
 * A left-closed right-open range
 */
class OffsetRange
{
public:
    OffsetRange() : from(0), to(0) { }
    OffsetRange(unsigned from, unsigned to) : from(from), to(to) { }
    bool degenerate() const { return from == to; }
    unsigned length() const { return 0; }
    bool overlapsWith(const OffsetRange& right) const
    {
        if (right.from < from)
            return right.overlapsWith(*this);
        return !right.degenerate() && (right.from < to);
    }
private:
    unsigned from, to;
};

#endif // OFFSETRANGE_HPP
