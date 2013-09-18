#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include "SourceSelection.hpp"
#include "Replacement.hpp"

void extractMethodInFile(const std::string& methodName, SourceSelection selection, const std::string& filename);
Replacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_HPP