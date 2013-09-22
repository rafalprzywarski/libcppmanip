#ifndef CPPMANIP_10F3A6DA37744054897720CE57342C12_HPP
#define CPPMANIP_10F3A6DA37744054897720CE57342C12_HPP
#include <stdexcept>

namespace cppmanip
{

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
    bool overlapsWith(const OffsetRange& right) const
    {
        if (right.from < from)
            return right.overlapsWith(*this);
        return (right.to != from) && (right.from < to);
    }
private:
    unsigned from, to;
};

}
#endif // CPPMANIP_10F3A6DA37744054897720CE57342C12_HPP
