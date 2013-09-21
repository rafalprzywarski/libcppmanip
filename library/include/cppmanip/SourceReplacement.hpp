#ifndef CPPMANIP_87BF0ADA8D254F05ADB329BD628E8995_HPP
#define CPPMANIP_87BF0ADA8D254F05ADB329BD628E8995_HPP
#include <string>
#include <vector>
#include <cppmanip/SourceLocation.hpp>

struct SourceReplacement
{
    SourceLocation from, to;
    std::string text;
};

typedef std::vector<SourceReplacement> SourceReplacements;

#endif // CPPMANIP_87BF0ADA8D254F05ADB329BD628E8995_HPP
