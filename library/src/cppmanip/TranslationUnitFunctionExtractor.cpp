#include "TranslationUnitFunctionExtractor.hpp"

namespace cppmanip
{

void TranslationUnitFunctionExtractor::handleTranslationUnit(clang::ASTContext& ctx) {
    auto& f = locateFunction(ctx);
    auto stmts = locateStatements(f);
    stmtExtractor.extractStatmentsFromFunction(stmts, f);
}

}

