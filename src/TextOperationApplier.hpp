#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>
#include <map>

class TextOperationApplier
{
public:
    std::string apply(const std::string& text);
    void insertTextAt(const std::string& text, unsigned offset);
private:
    std::map<unsigned, std::string, std::greater<unsigned> > insertions;
};

#endif // TEXTOPERATIONAPPLIER_HPP
