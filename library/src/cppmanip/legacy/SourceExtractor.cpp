#include "SourceExtractor.hpp"
#include <cppmanip/query/getStmtLocationRange.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>

namespace cppmanip
{
namespace legacy
{

std::string SourceExtractor::getSource(clang::SourceRange range) 
{
    return getSourceText(range.getBegin(), getLength(range));
}

std::string SourceExtractor::getSourceText(clang::SourceLocation loc, unsigned length)
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return std::string(text, length);
}

unsigned int SourceExtractor::getLength(clang::SourceRange r)
{
    return sourceManager.getFileOffset(r.getEnd()) - sourceManager.getFileOffset(r.getBegin());
}

}
}
