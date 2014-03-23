#ifndef CPPMANIP_7D4D105E3F9E45ADBF3E333076E95F16_HPP
#define CPPMANIP_7D4D105E3F9E45ADBF3E333076E95F16_HPP
#include <cppmanip/ast/SourceOffset.hpp>

namespace clang
{
class SourceManager;
class Stmt;
}

namespace cppmanip
{
namespace clangutil
{

ast::SourceOffsetRanges getStmtSpecificOffsetRanges(clang::SourceManager& sourceManager, clang::Stmt& stmt);

}
}
#endif // CPPMANIP_7D4D105E3F9E45ADBF3E333076E95F16_HPP
