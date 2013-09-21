#include "TextReplacementRecorder.hpp"

namespace cppmanip
{

void TextReplacementRecorder::replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to)
{
    SourceReplacement r;
    r.text = replacement;
    r.from = getOffsetFromSourceLocation(from);
    r.to = getOffsetFromSourceLocation(to);
    replacements.push_back(r);
}

SourceReplacements TextReplacementRecorder::getReplacements() const
{
    return replacements;
}

}
