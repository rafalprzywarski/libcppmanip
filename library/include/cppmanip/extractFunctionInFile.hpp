#ifndef CPPMANIP_5EDD069F04534471B823B01EB6DDFDD2_HPP
#define CPPMANIP_5EDD069F04534471B823B01EB6DDFDD2_HPP
#include <string>
#include <cppmanip/SourceSelection.hpp>
#include <cppmanip/SourceReplacement.hpp>
#include <cppmanip/ExtractMethodError.hpp>

namespace cppmanip
{
namespace boundary
{

SourceReplacements extractFunctionInFile(const std::string& functionName, SourceSelection selection, const std::string& filename);

}
}
#endif /* CPPMANIP_5EDD069F04534471B823B01EB6DDFDD2_HPP */