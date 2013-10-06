#ifndef CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#define CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
#include <cppmanip/ast/SourceOffset.hpp>

namespace clang
{
class SourceManager;
class Stmt;
}

namespace cppmanip
{
namespace query
{

ast::SourceOffsetRange getStmtOffsetRange(clang::SourceManager& sourceManager, clang::Stmt& stmt);

}
}
#endif // CPPMANIP_A030D04050524A5A9F75C404C32E9090_HPP
