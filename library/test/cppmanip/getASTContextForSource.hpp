#ifndef CPPMANIP_33B1095BD3F84848840FB242A7701A23_HPP
#define CPPMANIP_33B1095BD3F84848840FB242A7701A23_HPP
#include <memory>
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace test
{

std::shared_ptr<clang::ASTContext> getASTContextForSource(const std::string& source);

}
}
#endif // CPPMANIP_33B1095BD3F84848840FB242A7701A23_HPP
