#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>
#include <map>

class OffsetRange;

class TextOperationApplier
{
public:
    std::string apply(const std::string& text);
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
        std::string applyAtOffset(unsigned offset, const std::string& s) const;
    private:
        unsigned removalLength;
        std::string insertionText;
    };
    std::map<unsigned, Replacement, std::greater<unsigned> > replacements;
    
    void verifyNoOverlappingRangesExist(const OffsetRange& r);
};

#endif // TEXTOPERATIONAPPLIER_HPP
