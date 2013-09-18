#ifndef TEXTREPLACER_HPP
#define TEXTREPLACER_HPP
#include "TextReplacementListener.hpp"

class TextReplacer : public TextReplacementListener
{
public:
    explicit TextReplacer(const std::string& text) : text(text) { }
    void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to);
    std::string getText() const { return text; }
private:
    std::string text;
};

#endif // TEXTREPLACER_HPP
