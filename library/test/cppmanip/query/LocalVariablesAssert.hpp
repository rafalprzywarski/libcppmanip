#ifndef CPPMANIP_3AAC7F40C7624426957CE712D2ED382D_HPP
#define CPPMANIP_3AAC7F40C7624426957CE712D2ED382D_HPP
#include <cppmanip/ast/LocalVariable.hpp>

namespace cppmanip
{
namespace query
{
namespace test
{

void expectEqUnordered(ast::LocalVariables found, std::vector<ast::LocalVariable> expected);
void expectEqOrdered(ast::LocalVariables found, std::vector<ast::LocalVariable> expected);

}
}
}
#endif // CPPMANIP_3AAC7F40C7624426957CE712D2ED382D_HPP
