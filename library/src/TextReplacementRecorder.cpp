#include "TextReplacementRecorder.hpp"

void TextReplacementRecorder::replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to)
{
    r = SourceReplacement();
    r->text = replacement;
    r->from = getOffsetFromSourceLocation(from);
    r->to = getOffsetFromSourceLocation(to);
}

SourceReplacements TextReplacementRecorder::getReplacements() const
{
    if (!r)
        return SourceReplacements();
    return { *r };
}
