#include "TextOperationApplier.hpp"

std::string TextOperationApplier::apply(const std::string& text)
{
    if (textToInsert.empty())
        return text;
    return text.substr(0, insertionOffset) + textToInsert + text.substr(insertionOffset);
}

void TextOperationApplier::insertTextAt(const std::string& text, unsigned offset)
{
    textToInsert = text;
    insertionOffset = offset;
}
