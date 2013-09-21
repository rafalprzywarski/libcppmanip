#ifndef CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#define CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
#include <clang/Tooling/Tooling.h>
#include "SourceRange.hpp"
#include "OffsetRange.hpp"

namespace cppmanip
{

class TextOperationApplier;

class MethodExtractorFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    MethodExtractorFrontendActionFactory(
        const std::string& extractedMethodName, SourceRange selection, TextOperationApplier& sourceOperations)
        : extractedMethodName(extractedMethodName), selection(selection.from, selection.to),
        sourceOperations(sourceOperations) { }

    virtual clang::FrontendAction* create();

private:
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
};

}
#endif // CPPMANIP_71A4C766A1F04C4BB01EFA68BF81545F_HPP
