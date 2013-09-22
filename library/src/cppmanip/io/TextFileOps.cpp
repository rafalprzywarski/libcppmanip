#include "TextFileOps.hpp"
#include <fstream>

namespace cppmanip
{

namespace io
{

std::string loadTextFromFile(const std::string& filename)
{
    std::ifstream f(filename);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

}
}
