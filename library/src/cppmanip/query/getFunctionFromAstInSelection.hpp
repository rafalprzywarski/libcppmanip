#ifndef CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#define CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
#include <cppmanip/LocationRange.hpp>
#include <cppmanip/ast/Function.hpp>
#include <clang/AST/Decl.h>

namespace cppmanip
{
namespace query
{

ast::Function getFunctionFromAstInSelection(clang::ASTContext& context, LocationRange selection);

}
}
#endif // CPPMANIP_9C814E877C7147EB88A24679CA5852C2_HPP
