#ifndef CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#define CPPMANIP_01077C995F92441DB2604431793CC665_HPP
#include <string>
#include <map>
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
        Replacement() : removalLength(0) { }
        void appendInsertionText(const std::string& s);
        void setRemovalLength(unsigned len);
        bool overlapsWithRangeAtOffset(const OffsetRange& r, unsigned offset) const;
        void applyAtOffset(unsigned offset, TextReplacementListener& listner) const;
    private:
        unsigned removalLength;
        std::string insertionText;
    };
    std::map<unsigned, Replacement, std::greater<unsigned> > replacements;

    void verifyNoOverlappingRangesExist(const OffsetRange& r);
};

}
#endif // CPPMANIP_01077C995F92441DB2604431793CC665_HPP
