#ifndef CPPMANIP_5D669DAA80544067971378BD1ED150FA_HPP
#define CPPMANIP_5D669DAA80544067971378BD1ED150FA_HPP
#include "TextReplacementListener.hpp"
#include <cppmanip/boundary/SourceReplacement.hpp>
#include <functional>

namespace cppmanip
{
namespace text
{

class TextReplacementRecorder : public TextReplacementListener<unsigned>
{
public:
    typedef std::function<boundary::SourceLocation(unsigned)> OffsetToSourceLocation;
    TextReplacementRecorder(OffsetToSourceLocation getSourceLocationFromOffset)
        : getSourceLocationFromOffset(getSourceLocationFromOffset) { }
    virtual void replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to);
    boundary::SourceReplacements getReplacements() const;
private:
    OffsetToSourceLocation getSourceLocationFromOffset;
    boundary::SourceReplacements replacements;
};

}
}
#endif // CPPMANIP_5D669DAA80544067971378BD1ED150FA_HPP
