#ifndef METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
#define METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
#include <clang/Tooling/Tooling.h>
#include <SourceSelection.hpp>
#include "OffsetRange.hpp"

class TextOperationApplier;

class MethodExtractorFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    MethodExtractorFrontendActionFactory(
        const std::string& extractedMethodName, SourceSelection selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection.from, selection.to),
        sourceOperations(sourceOperations) { }

    virtual clang::FrontendAction* create();

private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

#endif // METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
