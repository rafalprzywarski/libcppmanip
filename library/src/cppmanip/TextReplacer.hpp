#ifndef CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
#define CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
#include "TextReplacementListener.hpp"

namespace cppmanip
{

class TextReplacer : public TextReplacementListener<unsigned>
{
public:
    explicit TextReplacer(const std::string& text) : text(text) { }
    void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to);
    std::string getText() const { return text; }
private:
    std::string text;
};

}
#endif // CPPMANIP_D9C114835E7740F0946533374E15CE6D_HPP
