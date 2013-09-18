#ifndef SOURCEREPLACEMENT_HPP
#define SOURCEREPLACEMENT_HPP
#include <string>
#include <vector>

struct SourceLocation
{
    unsigned row, col;
    SourceLocation() : row(), col() { }
};

struct SourceReplacement
{
    SourceLocation from, to;
    std::string text;
};

typedef std::vector<SourceReplacement> SourceReplacements;

#endif // SOURCEREPLACEMENT_HPP
