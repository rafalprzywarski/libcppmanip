#ifndef CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#define CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#include "LocationRange.hpp"
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Stmt.h>

namespace cppmanip
{

clang::SourceRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);
LocationRange getStmtLocationRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);

}
#endif // CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
