#ifndef TEXTOPERATIONAPPLIER_HPP
#define TEXTOPERATIONAPPLIER_HPP
#include <string>
#include <deque>

class TextOperationApplier
{
public:
    std::string apply(const std::string& text);
    void insertTextAt(const std::string& text, unsigned offset);
private:
    struct Insertion
    {
        std::string text;
        unsigned offset;
        
        Insertion(const std::string& text, unsigned offset);
    };
    std::deque<Insertion> insertions;
};

#endif // TEXTOPERATIONAPPLIER_HPP
