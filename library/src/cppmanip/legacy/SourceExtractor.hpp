#ifndef CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#define CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
#include <string>
#include <clang/Basic/SourceLocation.h>

namespace clang
{
class VarDecl;
class SourceManager;
}

namespace cppmanip
{
namespace legacy
{

class SourceExtractor
{
public:
    SourceExtractor(clang::SourceManager& sourceManager) : sourceManager(sourceManager) { }

    std::string getSource(clang::SourceRange range);

private:
    clang::SourceManager& sourceManager;

    unsigned getLength(clang::SourceRange r);
    std::string getSourceText(clang::SourceLocation loc, unsigned length);
};

}
}
#endif // CPPMANIP_215653695B8744B6B4E2B7B6AC37D277_HPP
