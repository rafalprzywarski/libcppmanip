#include "getSourceFromRange.hpp"
#include <cppmanip/query/getStmtLocationRange.hpp>

namespace cppmanip
{
namespace query
{
namespace
{

std::string getSourceText(clang::SourceManager& sourceManager, clang::SourceLocation loc, unsigned length)
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return std::string(text, length);
}

unsigned getLength(clang::SourceManager& sourceManager, clang::SourceRange r)
{
    return sourceManager.getFileOffset(r.getEnd()) - sourceManager.getFileOffset(r.getBegin());
}
}

std::string getSourceFromRange(clang::SourceManager& sourceManager, clang::SourceRange range)
{
    return getSourceText(sourceManager, range.getBegin(), getLength(sourceManager, range));
}

}
}
