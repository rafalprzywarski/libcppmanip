#ifndef CPPMANIP_D6F135FFE72248C89F6E9B9376F7F442_HPP
#define CPPMANIP_D6F135FFE72248C89F6E9B9376F7F442_HPP
#include <string>
#include <cppmanip/ast/Statement.hpp>

namespace cppmanip
{
namespace format
{

std::string formatStatements(ast::StatementRange stmts, unsigned indentationWidth);

}
}
#endif // CPPMANIP_D6F135FFE72248C89F6E9B9376F7F442_HPP
