#include "getFunctionDefinitionLocation.hpp"
#include <clang/AST/Decl.h>

namespace cppmanip
{
namespace query
{

clang::SourceLocation getFunctionDefinitionLocation(clang::FunctionDecl& f)
{
    return f.getLocStart();
}

}
}
