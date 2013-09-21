#ifndef CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
#define CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
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


#endif // CPPMANIP_FB9B63A4F1F94D448C896416220B5F93_HPP
