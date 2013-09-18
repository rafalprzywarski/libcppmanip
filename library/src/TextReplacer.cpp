#include "TextReplacer.hpp"

void TextReplacer::replaceWithTextInRange(const std::string& replacement, unsigned int from, unsigned int to) {
    text = text.substr(0, from) + replacement + text.substr(to);
}
