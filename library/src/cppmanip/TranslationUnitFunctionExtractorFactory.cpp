#include "TranslationUnitFunctionExtractorFactory.hpp"
#include "legacy/DelayedFunctionExtractor.hpp"
#include "TranslationUnitFunctionExtractor.hpp"
#include "query/getFunctionFromAstInSelection.hpp"
#include "query/findSelectedStatementsInFunction.hpp"
#include "query/getStmtLocationRange.hpp"
#include "query/findLocalVariablesRequiredForStmts.hpp"
#include "query/findVariablesDeclaredByAndUsedAfterStmts.hpp"
#include "query/getSourceFromRange.hpp"
#include "format/printFunction.hpp"

namespace cppmanip
{

clangutil::HandleTranslationUnit TranslationUnitFunctionExtractorFactory::createFunctionExtractor(
    const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
{
    using std::bind;
    using namespace std::placeholders;
    struct WithDeps
    {
        TranslationUnitFunctionExtractor functionExtractor;
        WithDeps(const std::string& extractedMethodName, LocationRange selection, text::OffsetBasedTextModifier& sourceOperations)
            : functionExtractor(
                bind(query::getFunctionFromAstInSelection, _1, selection),
                [=](ast::FunctionPtr decl) {
                    return query::findSelectedStatementsInFunction(decl->getDecl(), [=](clang::Stmt& s) {
                        return query::getStmtLocationRange(decl->getDecl().getASTContext().getSourceManager(), s).overlapsWith(selection);
                    });
                },
                [&](clang::ASTContext& ctx) {
                    auto& sourceManager = ctx.getSourceManager();
                    return std::make_shared<legacy::DelayedFunctionExtractor>(
                        sourceOperations, format::printFunctionCall, format::printFunctionDefinition,
                        bind(&clang::SourceManager::getFileOffset, &sourceManager, _1),
                        bind(query::getStmtsRange, std::ref(sourceManager), _1),
                        query::findLocalVariablesRequiredForStmts,
                        query::findVariablesDeclaredByAndUsedAfterStmts, extractedMethodName,
                        bind(query::getSourceFromRange, std::ref(sourceManager), _1));
                }) { }
    };
    auto withDeps = std::make_shared<WithDeps>(extractedMethodName, selection, sourceOperations);
    std::shared_ptr<TranslationUnitFunctionExtractor> handler{withDeps, &withDeps->functionExtractor};
    return [=](clang::ASTContext& ctx){ handler->handleTranslationUnit(ctx); };
}

}

