#include "MethodExtractorVisitor.hpp"

MethodExtractorVisitor::MethodExtractorVisitor(
    const std::string& extractedMethodName, MethodExtractor& extractor, StatementLocator& stmtLocator)
    : extractedMethodName(extractedMethodName), extractor(extractor), stmtLocator(stmtLocator) { }

bool MethodExtractorVisitor::VisitFunctionDecl(clang::FunctionDecl* decl)
{
    auto stmts = stmtLocator.findStatementsInFunction(*decl);
    if (!stmts)
        return true;
    extractor.extractStatmentsFromFunctionIntoNewFunction(stmts, *decl, extractedMethodName);
    return false;
}

