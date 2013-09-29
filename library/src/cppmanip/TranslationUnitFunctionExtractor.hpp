#ifndef CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#define CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
#include "StatementExtractor.hpp"

namespace cppmanip
{

class TranslationUnitFunctionExtractor
{
public:
    typedef std::function<clang::FunctionDecl&(clang::ASTContext&)> LocateFunction;
    typedef std::function<clang::StmtRange(clang::FunctionDecl&)> LocateStatements;
    typedef std::function<StatementExtractorPtr(clang::ASTContext& ctx)> CreateStatementExtractor;
    TranslationUnitFunctionExtractor(LocateFunction locateFunction, LocateStatements locateStatements, CreateStatementExtractor createStmtExtractor)
        : locateFunction(locateFunction), locateStatements(locateStatements), createStmtExtractor(createStmtExtractor) { }
    virtual void handleTranslationUnit(clang::ASTContext& ctx);
private:
    LocateFunction locateFunction;
    LocateStatements locateStatements;
    CreateStatementExtractor createStmtExtractor;
};

}
#endif // CPPMANIP_F6D4A1972E414BE5B79D1D592783B2D2_HPP
