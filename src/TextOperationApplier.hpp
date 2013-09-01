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
    struct Range
    {
        unsigned from, to;
        Range() : from(0), to(0) { }
        Range(unsigned from, unsigned to);
        bool overlapsWith(const Range& r) const;
    };
    std::map<unsigned, std::string, std::greater<unsigned> > insertions;
    std::map<unsigned, Range, std::greater<unsigned> > removals;
    
    void verifyNoOverlappingRangesExist(const Range& r);
};

#endif // TEXTOPERATIONAPPLIER_HPP
