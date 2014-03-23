#include "getStmtSpecificOffsetRanges.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/StmtCXX.h>
#include <clang/Basic/SourceManager.h>

namespace cppmanip
{
namespace clangutil
{

ast::SourceOffsetRanges getStmtSpecificOffsetRanges(clang::SourceManager& sourceManager, clang::Stmt& stmt)
{
    auto tryStmt = clang::dyn_cast<clang::CXXTryStmt>(&stmt);
    if (!tryStmt)
        return {};
    auto catchStmt = tryStmt->getHandler(0);
    std::array<std::array<clang::SourceLocation, 2>, 3> locRanges = { {
        { tryStmt->getTryLoc(), tryStmt->getTryBlock()->getLBracLoc().getLocWithOffset(1) },
        { catchStmt->getCatchLoc(), catchStmt->getHandlerBlock()->getLocStart().getLocWithOffset(1) },
        { catchStmt->getHandlerBlock()->getLocEnd(), catchStmt->getHandlerBlock()->getLocEnd().getLocWithOffset(1) }
    } };
    ast::SourceOffsetRanges ranges;
    ranges.reserve(locRanges.size());
    for (auto const& r : locRanges)
        ranges.emplace_back(sourceManager.getFileOffset(r[0]), sourceManager.getFileOffset(r[1]));
    return ranges;
}

}
}
