#ifndef TEXTFILEOPS_HPP
#define TEXTFILEOPS_HPP
#include <string>

std::string loadTextFromFile(const std::string& filename);
void writeTextToFile(const std::string& text, const std::string& filename);

#endif // TEXTFILEOPS_HPP