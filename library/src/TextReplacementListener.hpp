#ifndef TEXTREPLACEMENTLISTENER_HPP
#define TEXTREPLACEMENTLISTENER_HPP
#include <string>

class TextReplacementListener
{
public:
    virtual ~TextReplacementListener() { }
    virtual void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to) = 0;
};

#endif // TEXTREPLACEMENTLISTENER_HPP
