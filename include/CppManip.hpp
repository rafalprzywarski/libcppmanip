#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include "SourceSelection.hpp"
#include "SourceReplacement.hpp"
#include "SourceRange.hpp"

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_HPP