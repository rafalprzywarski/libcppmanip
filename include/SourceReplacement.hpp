#ifndef SOURCEREPLACEMENT_HPP
#define SOURCEREPLACEMENT_HPP
#include <string>
#include <vector>

struct SourceLocation
{
    unsigned row;
    unsigned col;
    SourceLocation() : SourceLocation(0,0) {}
    SourceLocation(unsigned row, unsigned col)
    : row(row), col(col) {}
};

struct SourceReplacement
{
    SourceLocation from, to;
    std::string text;
};

typedef std::vector<SourceReplacement> SourceReplacements;

#endif // SOURCEREPLACEMENT_HPP
