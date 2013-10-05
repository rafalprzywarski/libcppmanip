#ifndef CPPMANIP_357717E398AA461688871605ED7F697E_HPP
#define CPPMANIP_357717E398AA461688871605ED7F697E_HPP
#include <ostream>

namespace cppmanip
{
namespace ast
{

typedef unsigned SourceOffset;

class SourceOffsetRange
{
public:
    SourceOffsetRange(SourceOffset from, SourceOffset to)
        : from(from), to(to) { }
    SourceOffset getFrom() const { return from; }
    SourceOffset getTo() const { return to; }
private:
    SourceOffset from, to;
};

inline bool operator==(const SourceOffsetRange& left, const SourceOffsetRange& right)
{
    return left.getFrom() == right.getFrom() && left.getTo() == right.getTo();
}

inline std::ostream& operator<<(std::ostream& os, const SourceOffsetRange& r)
{
    return os << "[" << r.getFrom() << "; " << r.getTo() << ")";
}

}
}
#endif // CPPMANIP_357717E398AA461688871605ED7F697E_HPP
