#ifndef CPPMANIP_F700977BB33C4599BFB514B446B105AF_HPP
#define CPPMANIP_F700977BB33C4599BFB514B446B105AF_HPP
#include <clang/Frontend/FrontendAction.h>
#include "OffsetRange.hpp"
#include "TextModifier.hpp"

namespace cppmanip
{

class MethodExtractorFrontendAction : public clang::ASTFrontendAction
{
public:
    MethodExtractorFrontendAction(
        const std::string& extractedMethodName, OffsetRange selection, OffsetBasedTextModifier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection), sourceOperations(sourceOperations) { }
    virtual clang::ASTConsumer* CreateASTConsumer(clang::CompilerInstance&, clang::StringRef);
private:
    std::string extractedMethodName;
    OffsetRange selection;
    OffsetBasedTextModifier& sourceOperations;
};

}
#endif // CPPMANIP_F700977BB33C4599BFB514B446B105AF_HPP
