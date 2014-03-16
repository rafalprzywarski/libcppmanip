#ifndef CPPMANIP_9FC72215254C46568B68337C10759069_HPP
#define CPPMANIP_9FC72215254C46568B68337C10759069_HPP
#include <cppmanip/ast/Statement.hpp>
#include "IsStatementSelected.hpp"

namespace cppmanip
{
namespace query
{

StatementSelected isStatementSelected(ast::StatementPtr stmt, ast::SourceOffsetRange selection);

}
}
#endif // CPPMANIP_9FC72215254C46568B68337C10759069_HPP
