#include "MethodExtractorFrontendActionFactory.hpp"
#include "MethodExtractorFrontendAction.hpp"

namespace cppmanip
{

clang::FrontendAction* MethodExtractorFrontendActionFactory::create() {
    return new MethodExtractorFrontendAction(extractedMethodName, selection, sourceOperations);
}

}
