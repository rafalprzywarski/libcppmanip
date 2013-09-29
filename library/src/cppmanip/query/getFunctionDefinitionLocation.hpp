#ifndef GETFUNCTIONDEFINITIONLOCATION_HPP
#define GETFUNCTIONDEFINITIONLOCATION_HPP
#include <clang/Basic/SourceLocation.h>

namespace clang { class FunctionDecl; }

namespace cppmanip
{
namespace query
{

clang::SourceLocation getFunctionDefinitionLocation(clang::FunctionDecl& f);

}
}
#endif // GETFUNCTIONDEFINITIONLOCATION_HPP
