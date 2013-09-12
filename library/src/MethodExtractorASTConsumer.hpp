#ifndef METHODEXTRACTORASTCONSUMER_HPP
#define METHODEXTRACTORASTCONSUMER_HPP
#include <clang/AST/ASTConsumer.h>
#include <string>
#include "OffsetRange.hpp"

class ExtractMethodListener;
class TextOperationApplier;

class MethodExtractorASTConsumer : public clang::ASTConsumer
{
public:
    MethodExtractorASTConsumer(
        const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations,
        ExtractMethodListener& listener)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations),
        listener(listener) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx);
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
    ExtractMethodListener& listener;
};

#endif // METHODEXTRACTORASTCONSUMER_HPP
