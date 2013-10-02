#ifndef CPPMANIP_0268B93F0DA54054804296C6E2EDEC6B_HPP
#define CPPMANIP_0268B93F0DA54054804296C6E2EDEC6B_HPP
#include <ostream>
namespace cppmanip
{
namespace ast
{

class SourceLocation
{
public:
    SourceLocation(unsigned row, unsigned col) : row(row), col(col) { }
    unsigned getRow() const { return row; }
    unsigned getCol() const { return col; }
private:
    unsigned row, col;
};

inline SourceLocation rowCol(unsigned row, unsigned col)
{
    return { row, col };
}

inline bool operator==(const SourceLocation& left, const SourceLocation& right)
{
    return left.getRow() == right.getRow() && left.getCol() == right.getCol();
}

inline bool operator<(const SourceLocation& left, const SourceLocation& right)
{
    if (left.getRow() == right.getRow())
        return left.getCol() < right.getCol();
    return left.getRow() < right.getRow();
}

inline std::ostream& operator<<(std::ostream& os, SourceLocation l)
{
    return os << "(" << l.getRow() << ", " << l.getCol() << ")";
}

}
}
#endif // CPPMANIP_0268B93F0DA54054804296C6E2EDEC6B_HPP
