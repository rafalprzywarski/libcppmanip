#include "MethodExtractorFrontendActionFactory.hpp"
#include "MethodExtractorFrontendAction.hpp"

clang::FrontendAction* MethodExtractorFrontendActionFactory::create() {
    return new MethodExtractorFrontendAction(extractedMethodName, selection, sourceOperations, listener);
}
