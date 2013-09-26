#ifndef CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#define CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#include "StatementExtractor.hpp"
#include "clangutil/TranslationUnitHandler.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractor : public clangutil::TranslationUnitHandler
{
public:
    typedef std::function<clang::FunctionDecl&(clang::ASTContext&)> LocateFunction;
    typedef std::function<clang::StmtRange(clang::FunctionDecl&)> LocateStatements;
    TranslationUnitFunctionExtractor(LocateFunction locateFunction, LocateStatements locateStatements, StatementExtractor& stmtExtractor)
        : locateFunction(locateFunction), locateStatements(locateStatements), stmtExtractor(stmtExtractor) { }
    virtual void handleTranslationUnit(clang::ASTContext& ctx);
private:
    LocateFunction locateFunction;
    LocateStatements locateStatements;
    StatementExtractor& stmtExtractor;
};

}
#endif // CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
