#ifndef CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
#define CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP

template <typename Position>
class TextModifier
{
public:
    virtual ~TextModifier() { }
    virtual void insertTextAt(const std::string& text, Position pos) = 0;
    virtual void removeTextInRange(Position from, Position to) = 0;
};

typedef TextModifier<unsigned> OffsetBasedTextModifier;

#endif // CPPMANIP_DC40597E542F4CDB995BD64C51CDB200_HPP
