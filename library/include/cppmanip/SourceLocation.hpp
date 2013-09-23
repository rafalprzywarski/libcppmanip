#ifndef CPPMANIP_33B7E112BA6B4F6995C3437C9A2FFE96_HPP
#define CPPMANIP_33B7E112BA6B4F6995C3437C9A2FFE96_HPP

namespace cppmanip
{

struct SourceLocation
{
    unsigned row, col;
    SourceLocation() : row(), col() { }
};

inline SourceLocation rowCol(unsigned row, unsigned col)
{
    SourceLocation loc;
    loc.row = row;
    loc.col = col;
    return loc;
}

}
#endif // CPPMANIP_33B7E112BA6B4F6995C3437C9A2FFE96_HPP
