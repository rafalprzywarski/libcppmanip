#ifndef CPPMANIP_669DE9B8657142C39CC3D13F4EB849E8_HPP
#define CPPMANIP_669DE9B8657142C39CC3D13F4EB849E8_HPP
#include <clang/AST/ASTContext.h>
#include <functional>

namespace cppmanip
{
namespace clangutil
{

typedef std::function<void(clang::ASTContext&)> HandleTranslationUnit;

}
}
#endif // CPPMANIP_669DE9B8657142C39CC3D13F4EB849E8_HPP
