#include "findSelectedStatementsInFunction.hpp"
#include <clang/AST/ASTContext.h>

namespace cppmanip
{
namespace query
{

ast::StatementRange findSelectedStatementsInFunction(
    const ast::Function& decl, std::function<bool(ast::StatementPtr)> isSelected)
{
    auto first = std::find_if(decl.getStatements().begin(), decl.getStatements().end(), isSelected);
    auto last = std::find_if(first, decl.getStatements().end(),
        [&](const ast::StatementPtr& s) { return !isSelected(s); });
    return {first, last};
}

}
}
