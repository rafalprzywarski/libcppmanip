#ifndef CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#define CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#include <string>
#include <clang/Basic/SourceLocation.h>

namespace clang
{
class SourceManager;
}

namespace cppmanip
{
namespace query
{

std::string getStmtsSource(clang::SourceManager& sourceManager, clang::SourceRange range);

}
}
#endif // CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
