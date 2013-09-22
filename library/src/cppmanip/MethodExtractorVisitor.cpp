#include "MethodExtractorVisitor.hpp"

namespace cppmanip
{

bool MethodExtractorVisitor::VisitFunctionDecl(clang::FunctionDecl* decl)
{
    auto stmts = stmtLocator.findStatementsInFunction(*decl);
    if (!stmts)
        return true;
    this->decl = decl;
    return false;
}

}
