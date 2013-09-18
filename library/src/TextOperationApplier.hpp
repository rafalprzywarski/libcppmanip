#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>
#include <map>

class OffsetRange;

class TextReplacementListener
{
public:
    virtual ~TextReplacementListener() { }
    virtual void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to) = 0;
};

class TextReplacer : public TextReplacementListener
{
public:
    explicit TextReplacer(const std::string& text) : text(text) { }
    void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to)
    {
        text = text.substr(0, from) + replacement + text.substr(to);
    }
    std::string getText() const { return text; }
private:
    std::string text;
};

class TextOperationApplier
{
public:
    void apply(TextReplacementListener& replacer);
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

#endif // TEXTOPERATIONAPPLIER_HPP
