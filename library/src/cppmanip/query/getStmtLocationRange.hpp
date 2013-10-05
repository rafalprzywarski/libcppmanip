#ifndef CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#define CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#include <cppmanip/LocationRange.hpp>
#include <cppmanip/ast/SourceOffset.hpp>
#include <cppmanip/ast/Statement.hpp>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Stmt.h>

namespace cppmanip
{
namespace query
{

clang::SourceRange getStmtRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);
LocationRange getStmtLocationRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);
ast::SourceOffsetRange getStmtOffsetRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);

clang::SourceRange getStmtsRange(clang::SourceManager& sourceManager, ast::StatementRange stmts);

}
}
#endif // CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
