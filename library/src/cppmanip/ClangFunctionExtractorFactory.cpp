#include "ClangFunctionExtractorFactory.hpp"
#include "ClangFunctionExtractor.hpp"
#include "text/TextOperationApplier.hpp"
#include "makeWithDependencies.hpp"

namespace cppmanip
{

std::shared_ptr<FunctionExtractor> ClangFunctionExtractorFactory::createFunctionExtractor()
{
    struct WithDeps
    {
        text::OffsetBasedTextOperationApplier textModifier;
        ClangFunctionExtractor extractor{textModifier};
    };
    return makeWithDependencies(&WithDeps::extractor);
}

}

