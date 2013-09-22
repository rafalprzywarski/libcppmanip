#ifndef CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#define CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#include <string>
#include <map>
#include <boost/optional.hpp>
#include "TextReplacementListener.hpp"

namespace cppmanip
{

class OffsetRange;

class TextOperationApplier
{
public:
    void apply(TextReplacementListener& replacer) const;
    void insertTextAt(const std::string& text, unsigned offset);
    void removeTextInRange(unsigned from, unsigned to);
private:
    class Replacement
    {
    public:
        void appendInsertionText(const std::string& s);
        void setRemovalEnd(unsigned end);
        bool overlapsWithRangeAtOffset(const OffsetRange& r, unsigned offset) const;
        void applyAtOffset(unsigned offset, TextReplacementListener& listner) const;
    private:
        boost::optional<unsigned> removalEnd;
        std::string insertionText;
        unsigned getRemovalEnd(unsigned offset) const;
    };
    std::map<unsigned, Replacement, std::greater<unsigned> > replacements;

    void verifyNoOverlappingRangesExist(const OffsetRange& r);
};

}
#endif // CPPMANIP_01077C995F92441DB2604431793CC665_HPP
