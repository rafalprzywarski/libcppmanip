#ifndef CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
#define CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
#include "TextReplacementListener.hpp"

namespace cppmanip
{

template <typename Position>
class TextReplacer : public TextReplacementListener<Position>
{
public:
    explicit TextReplacer(const std::string& text) : text(text) { }
    void replaceWithTextInRange(const std::string& replacement, Position from, Position to)
    {
        text = text.substr(0, from) + replacement + text.substr(to);
    }
    std::string getText() const { return text; }
private:
    std::string text;
};

}
#endif // CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
