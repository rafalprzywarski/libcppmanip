#ifndef METHODEXTRACTORASTCONSUMER_HPP
#define METHODEXTRACTORASTCONSUMER_HPP
#include <clang/AST/ASTConsumer.h>
#include <string>
#include "OffsetRange.hpp"

class TextOperationApplier;

class MethodExtractorASTConsumer : public clang::ASTConsumer
{
public:
    MethodExtractorASTConsumer(const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx);
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

#endif // METHODEXTRACTORASTCONSUMER_HPP
