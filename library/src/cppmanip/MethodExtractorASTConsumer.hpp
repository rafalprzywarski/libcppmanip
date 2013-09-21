#ifndef CPPMANIP_B6BFCC681F474EA1B1E5AD0A2D9B3126_HPP
#define CPPMANIP_B6BFCC681F474EA1B1E5AD0A2D9B3126_HPP
#include <clang/AST/ASTConsumer.h>
#include <string>
#include "OffsetRange.hpp"

namespace cppmanip
{

class TextOperationApplier;

class MethodExtractorASTConsumer : public clang::ASTConsumer
{
public:
    MethodExtractorASTConsumer(
        const std::string& extractedMethodName, OffsetRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual void HandleTranslationUnit(clang::ASTContext& ctx);
private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

}
#endif // CPPMANIP_B6BFCC681F474EA1B1E5AD0A2D9B3126_HPP
