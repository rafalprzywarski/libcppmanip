#include "AstGateway.hpp"
#include "runTranslationUnitHandlerOnFile.hpp"
#include <cppmanip/clangutil/getFunctionFromAstInSelection.hpp>
#include <cppmanip/clangutil/getFunctionStatements.hpp>
#include <cppmanip/clangutil/getStmtOffsetRange.hpp>

namespace cppmanip
{
namespace clangutil
{

ast::FunctionPtr AstGateway::getFunctionInSelectionFromFile(ast::SourceOffsetRange selection, const std::string& filename)
{
    ast::FunctionPtr f;
    runTranslationUnitHandlerOnFile([&](clang::ASTContext& ctx)
    {
        using std::placeholders::_1;
        f = getFunctionFromAstInSelection(ctx, selection, std::bind(getFunctionStatements, _1, getStmtOffsetRange));
    }, filename);
    return f;
}

}
}

