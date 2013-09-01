#ifndef OFFSETRANGE_HPP
#define OFFSETRANGE_HPP

class OffsetRange
{
public:
    OffsetRange() { }
    OffsetRange(unsigned, unsigned) { }
    bool empty() const { return true; }
    unsigned length() const { return 0; }
    bool overlapsWith(const OffsetRange& ) { }
};

#endif // OFFSETRANGE_HPP
