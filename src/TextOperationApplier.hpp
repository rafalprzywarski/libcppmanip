#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>
#include <map>

class TextOperationApplier
{
public:
    std::string apply(const std::string& text);
    void insertTextAt(const std::string& text, unsigned offset);
    void removeTextInRange(unsigned from, unsigned to);
private:
    class Range
    {
    public:
        Range() : from(0), to(0) { }
        Range(unsigned from, unsigned to);
        bool overlapsWith(const Range& r) const;
        bool empty() const { return from == to; }
        unsigned length() const { return to - from; }
    private:
        unsigned from, to;
    };
    class Replacement
    {
    public:
        Replacement() : removalLength(0) { }
        void appendInsertionText(const std::string& s);
        void setRemovalLength(unsigned len);
        bool overlapsWithRangeAtOffset(const Range& r, unsigned offset) const;
        std::string applyAtOffset(unsigned offset, const std::string& s) const;
    private:
        unsigned removalLength;
        std::string insertionText;
    };
    std::map<unsigned, Replacement, std::greater<unsigned> > replacements;
    
    void verifyNoOverlappingRangesExist(const Range& r);
};

#endif // TEXTOPERATIONAPPLIER_HPP
