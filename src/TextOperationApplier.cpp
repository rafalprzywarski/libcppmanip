#include "TextOperationApplier.hpp"

std::string TextOperationApplier::apply(const std::string& text)
{
    if (insertions.empty())
        return text;
    auto copy = text;
    for (auto const& ins : insertions)
    {
        unsigned insertionOffset = ins.offset;
        auto textToInsert = ins.text;
        copy = copy.substr(0, insertionOffset) + textToInsert + copy.substr(insertionOffset);
    }
    return copy;
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    insertions.push_front({text, offset});
}

TextOperationApplier::Insertion::Insertion(const std::string& text, unsigned offset)
    : text(text), offset(offset) { }
