#ifndef SOURCELOCATIONCONVERTER_HPP
#define SOURCELOCATIONCONVERTER_HPP
#include <cppmanip/SourceReplacement.hpp>

class SourceLocationConverter
{
public:
    SourceLocationConverter(const std::string& source) : source(source) { }
    unsigned getOffsetFromLocation(SourceLocation loc) const;
private:
    std::string source;
    unsigned int calcRowOffset(unsigned int row) const;
};

#endif // SOURCELOCATIONCONVERTER_HPP
