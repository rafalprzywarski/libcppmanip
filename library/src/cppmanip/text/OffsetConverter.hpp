#ifndef CPPMANIP_D2CBE39E835843F3956DC75AC0955354_HPP
#define CPPMANIP_D2CBE39E835843F3956DC75AC0955354_HPP
#include <cppmanip/boundary/SourceReplacement.hpp>

namespace cppmanip
{
namespace text
{

class OffsetConverter
{
public:
    OffsetConverter(const std::string& source) : source(source) { }
    boundary::SourceLocation getLocationFromOffset(unsigned offset) const;
private:
    const std::string source;
    void validateOffset(unsigned int offset) const;
    unsigned int calcColumn(unsigned int offset) const;
    unsigned int calcRow(unsigned int offset) const ;
};

}
}
#endif // CPPMANIP_D2CBE39E835843F3956DC75AC0955354_HPP
