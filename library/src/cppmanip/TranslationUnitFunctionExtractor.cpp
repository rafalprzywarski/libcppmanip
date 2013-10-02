#include "TranslationUnitFunctionExtractor.hpp"

namespace cppmanip
{

void TranslationUnitFunctionExtractor::handleTranslationUnit(clang::ASTContext& ctx) {
    auto f = locateFunction(ctx);
    auto stmts = locateStatements(f);
    createStmtExtractor(ctx)->extractStatmentsFromFunction(stmts, f);
}

}

