#ifndef OFFSETCONVERTER_HPP
#define OFFSETCONVERTER_HPP
#include <SourceReplacement.hpp>

class OffsetConverter
{
public:
    OffsetConverter(const std::string& source) : source(source) { }
    SourceLocation getLocationFromOffset(unsigned offset) const;
private:
    std::string source;
};

#endif // OFFSETCONVERTER_HPP
