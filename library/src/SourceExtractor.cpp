#include "SourceExtractor.hpp"

clang::SourceRange SourceExtractor::getCorrectSourceRange(clang::ConstStmtRange stmts) 
{
    clang::SourceRange r;
    r.setBegin(getCorrectSourceRange(**stmts).getBegin());
    for (auto s : stmts)
        r.setEnd(getCorrectSourceRange(*s).getEnd());
    return r;
}
    
std::string SourceExtractor::getSource(clang::SourceRange range) 
{
    return std::string(getSourceText(range.getBegin()), getLength(range));
}

const char* SourceExtractor::getSourceText(clang::SourceLocation loc) 
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return text;
}

OffsetRange SourceExtractor::getOffsetRange(clang::SourceRange r)
{
    return {getOffset(r.getBegin()), getOffset(r.getEnd())};
}

unsigned int SourceExtractor::getOffset(clang::SourceLocation loc)
{
    return sourceManager.getFileOffset(loc);
}

bool SourceExtractor::isLocationFromMainFile(clang::SourceLocation loc)
{
    return sourceManager.isFromMainFile(loc);
}
