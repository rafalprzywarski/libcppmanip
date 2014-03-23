#ifndef CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
#define CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
#include <cppmanip/ast/Statement.hpp>
#include <cppmanip/ast/SourceOffset.hpp>

namespace clang { class FunctionDecl; class SourceManager; class Stmt; }

namespace cppmanip
{
namespace clangutil
{

typedef std::function<ast::SourceOffsetRange(clang::SourceManager&, clang::Stmt&)> GetStatementRange;
typedef std::function<ast::SourceOffsetRanges(clang::SourceManager&, clang::Stmt&)> GetStatementSpecificRanges;
ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange, GetStatementSpecificRanges getStmtSpecificRanges);

}
}
#endif // CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
