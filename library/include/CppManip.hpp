#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include <cppmanip/SourceSelection.hpp>
#include <cppmanip/SourceReplacement.hpp>

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_HPP