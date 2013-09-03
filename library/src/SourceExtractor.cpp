#include "SourceExtractor.hpp"

clang::SourceRange SourceExtractor::getCorrectSourceRange(clang::ConstStmtRange stmts) 
{
    clang::SourceRange r;
    r.setBegin(getCorrectSourceRange(**stmts).getBegin());
    for (auto s : stmts)
        r.setEnd(getCorrectSourceRange(*s).getEnd());
    return r;
}
    
std::string SourceExtractor::getSource(clang::ConstStmtRange stmts) 
{
    auto range = getCorrectSourceRange(stmts);
    return std::string(getText(range.getBegin()), rangeLength(range));
}

const char* SourceExtractor::getText(clang::SourceLocation loc) 
{
    auto invalid = true;
    auto text = sourceManager.getCharacterData(loc, &invalid);
    if (invalid)
        throw std::runtime_error("cannot get characted data");
    return text;
}
