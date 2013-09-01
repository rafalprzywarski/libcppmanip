#ifndef OFFSETRANGE_HPP
#define OFFSETRANGE_HPP

/**
 * A left-closed right-open range
 */
class OffsetRange
{
public:
    OffsetRange() { }
    OffsetRange(unsigned, unsigned) { }
    bool degenerate() const { return true; }
    unsigned length() const { return 0; }
    bool overlapsWith(const OffsetRange& ) { }
};

#endif // OFFSETRANGE_HPP
