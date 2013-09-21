#ifndef CPPMANIP_B7961154F94A4B4090C2B1E100230246_HPP
#define CPPMANIP_B7961154F94A4B4090C2B1E100230246_HPP
#include <string>
#include <cppmanip/SourceSelection.hpp>
#include <cppmanip/SourceReplacement.hpp>
#include <cppmanip/ExtractMethodError.hpp>

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

#endif // CPPMANIP_B7961154F94A4B4090C2B1E100230246_HPP