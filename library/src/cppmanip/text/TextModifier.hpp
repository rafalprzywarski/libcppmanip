#ifndef CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
#define CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
#include "TextReplacementListener.hpp"
#include <memory>

namespace cppmanip
{
namespace text
{

template <typename Position>
struct Replacement
{
    Position from, to;
    std::string text;
    Replacement() : from(), to() { }
};

template <typename Position>
using Replacements = std::vector<Replacement<Position>>;

template <typename Position>
class TextModifier
{
public:
    virtual ~TextModifier() { }
    virtual void insertTextAt(const std::string& text, Position pos) = 0;
    virtual void removeTextInRange(Position from, Position to) = 0;
    virtual void apply(TextReplacementListener<Position>& replacer) const = 0;
    virtual Replacements<Position> getReplacements() const = 0;
};

typedef TextModifier<unsigned> OffsetBasedTextModifier;
typedef std::shared_ptr<OffsetBasedTextModifier> OffsetBasedTextModifierPtr;

}
}
#endif // CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
