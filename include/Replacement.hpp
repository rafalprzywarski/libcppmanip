#ifndef REPLACEMENT_HPP
#define REPLACEMENT_HPP
#include <string>
#include <vector>

struct SourceLocation
{
    unsigned from, to;
    SourceLocation() : from(), to() { }
};

struct Replacement
{
    SourceLocation from, to;
    std::string text;
};

typedef std::vector<Replacement> Replacements;

#endif // REPLACEMENT_HPP
