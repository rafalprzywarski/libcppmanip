#ifndef METHODEXTRACTORVISITOR_HPP
#define METHODEXTRACTORVISITOR_HPP
#include <clang/AST/RecursiveASTVisitor.h>
#include "MethodExtractor.hpp"
#include "StatementLocator.hpp"

class MethodExtractorVisitor : public clang::RecursiveASTVisitor<MethodExtractorVisitor>
{
public:
    MethodExtractorVisitor(const std::string& extractedMethodName, MethodExtractor& extractor, StatementLocator& stmtLocator);

    bool VisitFunctionDecl(clang::FunctionDecl* decl);

private:
    std::string extractedMethodName;
    MethodExtractor& extractor;
    StatementLocator& stmtLocator;
};


#endif // METHODEXTRACTORVISITOR_HPP
