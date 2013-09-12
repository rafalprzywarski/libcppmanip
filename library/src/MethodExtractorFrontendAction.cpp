#include "MethodExtractorFrontendAction.hpp"
#include "MethodExtractorASTConsumer.hpp"

clang::ASTConsumer* MethodExtractorFrontendAction::CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef)
{
    return new MethodExtractorASTConsumer(extractedMethodName, selection, sourceOperations, listener);
}
