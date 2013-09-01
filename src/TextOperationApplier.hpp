#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>

class TextOperationApplier
{
public:
    std::string apply(const std::string& text);
    void insertTextAt(const std::string& text, unsigned offset);
private:
    std::string textToInsert;
    unsigned insertionOffset;
};

#endif // TEXTOPERATIONAPPLIER_HPP
