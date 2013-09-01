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
    std::map<unsigned, std::string, std::greater<unsigned> > insertions;
    std::map<unsigned, unsigned, std::greater<unsigned> > removals;
};

#endif // TEXTOPERATIONAPPLIER_HPP
