#include "TextFileOps.hpp"
#include <fstream>

namespace cppmanip
{

std::string loadTextFromFile(const std::string& filename)
{
    std::ifstream f(filename);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

void writeTextToFile(const std::string& text, const std::string& filename)
{
    std::ofstream f(filename);
    f << text;
}

}
