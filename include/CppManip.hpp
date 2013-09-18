#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include "SourceSelection.hpp"
#include "SourceReplacement.hpp"
#include "SourceRange.hpp"

void extractMethodInFile(const std::string& methodName, SourceRange selection, const std::string& filename);
SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_HPP