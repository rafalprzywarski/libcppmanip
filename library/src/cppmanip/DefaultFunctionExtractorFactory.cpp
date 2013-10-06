#include "DefaultFunctionExtractorFactory.hpp"
#include "FileBasedStatementLocator.hpp"
#include "clangutil/AstGateway.hpp"
#include "text/SourceLocationConverter.hpp"
#include "io/TextFileOps.hpp"
#include "query/findSelectedStatementsInFunction.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "NoReturnFunctionExtractionValidator.hpp"

namespace cppmanip
{

DefaultFunctionExtractorPtr DefaultFunctionExtractorFactory::create(const std::string& filename)
{
    auto sourceLocationConverter = std::make_shared<text::SourceLocationConverter>(io::loadTextFromFile(filename));
    auto getOffsetFromLocation = [=](const boundary::SourceLocation& loc) { return sourceLocationConverter->getOffsetFromLocation(loc); };
    auto astGateway = std::make_shared<clangutil::AstGateway>();
    auto stmtLocator = std::make_shared<FileBasedStatementLocator>(filename, astGateway, getOffsetFromLocation, query::findSelectedStatementsInFunction);
    auto validator = std::make_shared<NoReturnFunctionExtractionValidator>(query::findVariablesDeclaredByAndUsedAfterStmts);
    return std::make_shared<DefaultFunctionExtractor>(stmtLocator, validator);
}

}
