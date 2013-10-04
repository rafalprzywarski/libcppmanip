#ifndef CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
#define CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
#include <cppmanip/ast/Statement.hpp>

namespace clang { class FunctionDecl; class SourceManager; class Stmt; }

namespace cppmanip
{
namespace query
{

typedef std::function<ast::SourceLocationRange(clang::SourceManager&, clang::Stmt&)> GetStatementRange;
ast::Statements getFunctionStatements(clang::FunctionDecl& f, GetStatementRange getStmtRange);

}
}
#endif // CPPMANIP_9A1A27D4400E4B219C57A5C10D2BF14F_HPP
