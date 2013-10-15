#include "DefaultFunctionExtractorFactory.hpp"
#include "FileBasedStatementLocator.hpp"
#include "clangutil/AstGateway.hpp"
#include "text/SourceLocationConverter.hpp"
#include "text/StrictOperationRecorder.hpp"
#include "text/convertReplacements.hpp"
#include "text/OffsetConverter.hpp"
#include "io/TextFileOps.hpp"
#include "query/findSelectedStatementsInFunction.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "query/findLocalVariablesRequiredForStmts.hpp"
#include "NoReturnFunctionExtractionValidator.hpp"
#include "format/DefaultReplacementFunctionPrinter.hpp"
#include "format/printFunction.hpp"
#include "format/formatStatements.hpp"
#include "DefaultFunctionReplacer.hpp"

namespace cppmanip
{

DefaultFunctionExtractorPtr DefaultFunctionExtractorFactory::createForFile(const std::string& filename)
{
    auto sourceCode = io::loadTextFromFile(filename);
    auto sourceLocationConverter = std::make_shared<text::SourceLocationConverter>(sourceCode);
    auto offsetConverter = std::make_shared<text::OffsetConverter>(sourceCode);
    auto getOffsetFromLocation = [=](const boundary::SourceLocation& loc) { return sourceLocationConverter->getOffsetFromLocation(loc); };
    auto astGateway = std::make_shared<clangutil::AstGateway>();
    auto stmtLocator = std::make_shared<FileBasedStatementLocator>(filename, astGateway, getOffsetFromLocation, query::findSelectedStatementsInFunction);
    auto validator = std::make_shared<NoReturnFunctionExtractionValidator>(query::findVariablesDeclaredByAndUsedAfterStmts);
    auto formatStatements = [](ast::StatementRange stmts) { return format::formatStatements(stmts, 4); };
    auto printer = std::make_shared<format::DefaultReplacementFunctionPrinter>(
        query::findLocalVariablesRequiredForStmts, formatStatements, format::printFunctionCall, format::printFunctionDefinition);
    auto operationRecoderFactory = []{ return std::make_shared<text::OffsetBasedStrictOperationRecorder>(); };
    auto convertReplacements = [=](const text::Replacements<ast::SourceOffset>& reps) {
        return text::convertReplacements(reps, [=](unsigned offset) { return offsetConverter->getLocationFromOffset(offset); });
    };
    auto replacer = std::make_shared<DefaultFunctionReplacer>(operationRecoderFactory, convertReplacements);
    return std::make_shared<DefaultFunctionExtractor>(stmtLocator, validator, printer, replacer);
}

}
