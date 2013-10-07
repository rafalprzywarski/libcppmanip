#ifndef CPPMANIP_C3A4C24E3E214156998B382FAFC9CE04_HPP
#define CPPMANIP_C3A4C24E3E214156998B382FAFC9CE04_HPP
#include <cppmanip/boundary/SourceReplacement.hpp>
#include <cppmanip/boundary/SourceSelection.hpp>
#include "StatementLocator.hpp"
#include "FunctionExtractionValidator.hpp"
#include "format/ReplacementFunctionPrinter.hpp"
#include <memory>

namespace cppmanip
{

class DefaultFunctionExtractor
{
public:
    DefaultFunctionExtractor(
        StatementLocatorPtr stmtLocator, FunctionExtractionValidatorPtr validator, format::ReplacementFunctionPrinterPtr functionPrinter)
        : stmtLocator(stmtLocator), validator(validator), functionPrinter(functionPrinter) { }
    virtual boundary::SourceReplacements extractFunctionFromSelectionInFile(
        const std::string& functionName, boundary::SourceSelection selection, const std::string& filename);
private:
    StatementLocatorPtr stmtLocator;
    FunctionExtractionValidatorPtr validator;
    format::ReplacementFunctionPrinterPtr functionPrinter;
};


typedef std::shared_ptr<DefaultFunctionExtractor> DefaultFunctionExtractorPtr;
}
#endif // CPPMANIP_C3A4C24E3E214156998B382FAFC9CE04_HPP
