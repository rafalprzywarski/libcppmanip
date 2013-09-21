#ifndef SOURCEREPLACEMENT_HPP
#define SOURCEREPLACEMENT_HPP
#include <string>
#include <vector>
#include "SourceLocation.hpp"

struct SourceReplacement
{
    SourceLocation from, to;
    std::string text;
};

typedef std::vector<SourceReplacement> SourceReplacements;

#endif // SOURCEREPLACEMENT_HPP
