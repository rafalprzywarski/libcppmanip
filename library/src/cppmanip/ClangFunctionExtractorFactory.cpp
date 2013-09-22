#include "ClangFunctionExtractorFactory.hpp"
#include "ClangFunctionExtractor.hpp"
#include "TextOperationApplier.hpp"
#include "makeWithDependencies.hpp"

namespace cppmanip
{

std::shared_ptr<FunctionExtractor> ClangFunctionExtractorFactory::createFunctionExtractor()
{
    struct WithDeps
    {
        OffsetBasedTextOperationApplier textModifier;
        ClangFunctionExtractor extractor{textModifier};
    };
    return makeWithDependencies(&WithDeps::extractor);
}

}

