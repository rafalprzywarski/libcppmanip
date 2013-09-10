#ifndef CPPMANIP_HPP
#define CPPMANIP_HPP
#include <string>
#include "SourceSelection.hpp"
#include "ExtractMethodListener.hpp"

void extractMethodInFile(const std::string& methodName, SourceSelection selection, const std::string& filename, ExtractMethodListener& listener);

#endif // CPPMANIP_HPP