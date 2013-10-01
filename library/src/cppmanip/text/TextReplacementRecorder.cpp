#include "TextReplacementRecorder.hpp"

namespace cppmanip
{
namespace text
{

void TextReplacementRecorder::replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to)
{
    boundary::SourceReplacement r;
    r.text = replacement;
    r.from = getSourceLocationFromOffset(from);
    r.to = getSourceLocationFromOffset(to);
    replacements.push_back(r);
}

boundary::SourceReplacements TextReplacementRecorder::getReplacements() const
{
    return replacements;
}

}
}
