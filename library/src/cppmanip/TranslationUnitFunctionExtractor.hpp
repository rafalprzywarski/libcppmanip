#ifndef CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#define CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#include "ClangFunctionLocator.hpp"
#include "StatementLocator.hpp"
#include "StatementExtractor.hpp"
#include "clangutil/TranslationUnitHandler.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractor : public clangutil::TranslationUnitHandler
{
public:
    TranslationUnitFunctionExtractor(ClangFunctionLocator& functionLocator, StatementLocator& stmtLocator, StatementExtractor& stmtExtractor)
        : functionLocator(functionLocator), stmtLocator(stmtLocator), stmtExtractor(stmtExtractor) { }
    virtual void handleTranslationUnit(clang::ASTContext& ctx);
private:
    ClangFunctionLocator& functionLocator;
    StatementLocator& stmtLocator;
    StatementExtractor& stmtExtractor;
};

}
#endif // CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
