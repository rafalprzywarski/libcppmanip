#ifndef TEXTREPLACEMENTRECORDER_HPP
#define TEXTREPLACEMENTRECORDER_HPP
#include "TextReplacementListener.hpp"
#include <SourceReplacement.hpp>
#include <boost/optional.hpp>

class TextReplacementRecorder : public TextReplacementListener
{
public:
    typedef std::function<SourceLocation(unsigned)> OffsetToSourceLocation;
    TextReplacementRecorder(OffsetToSourceLocation getOffsetFromSourceLocation)
        : getOffsetFromSourceLocation(getOffsetFromSourceLocation) { }
    virtual void replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to);
    SourceReplacements getReplacements() const;
private:
    OffsetToSourceLocation getOffsetFromSourceLocation;
    boost::optional<SourceReplacement> r;
};

#endif // TEXTREPLACEMENTRECORDER_HPP
