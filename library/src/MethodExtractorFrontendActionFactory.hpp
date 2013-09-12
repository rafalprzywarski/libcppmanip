#ifndef METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
#define METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
#include <clang/Tooling/Tooling.h>
#include <SourceSelection.hpp>
#include "OffsetRange.hpp"

class ExtractMethodListener;
class TextOperationApplier;

class MethodExtractorFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    MethodExtractorFrontendActionFactory(
        const std::string& extractedMethodName, SourceSelection selection, TextOperationApplier& sourceOperations,
        ExtractMethodListener& listener)
        : extractedMethodName(extractedMethodName), selection(selection.from, selection.to),
        sourceOperations(sourceOperations), listener(listener) { }

    virtual clang::FrontendAction* create();

private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
    ExtractMethodListener& listener;
};

#endif // METHODEXTRACTORFRONTENDACTIONFACTORY_HPP
