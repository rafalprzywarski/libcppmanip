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
    void validateOffset(unsigned int offset) const;
    unsigned int calcColumn(unsigned int offset) const;
    unsigned int calcRow(unsigned int offset) const ;
};

#endif // OFFSETCONVERTER_HPP
