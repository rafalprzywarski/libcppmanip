#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include "SourceSelection.hpp"

void extractMethodInFile(const std::string& methodName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_HPP