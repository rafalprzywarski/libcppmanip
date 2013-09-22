#include "MethodExtractorASTConsumer.hpp"
#include "SourceExtractor.hpp"
#include "MethodExtractor.hpp"
#include "PrettyFunctionPrinter.hpp"
#include "NaiveStatementLocator.hpp"
#include "DelayedMethodExtractor.hpp"
#include "MethodExtractorVisitor.hpp"
#include "NaiveLocalVariableLocator.hpp"
#include <clang/AST/ASTContext.h>

namespace cppmanip
{

class ClangFunctionLocator
{
public:
    virtual ~ClangFunctionLocator() { }
    virtual clang::FunctionDecl& getFunctionContainingSelection(OffsetRange selection) = 0;
};

class NaiveClangFunctionLocator : public ClangFunctionLocator
{
public:
    NaiveClangFunctionLocator(clang::ASTContext& ctx) : ctx(ctx) { }
    clang::FunctionDecl& getFunctionContainingSelection(OffsetRange selection)
    {
        SourceExtractor sourceExtractor(ctx.getSourceManager());
        NaiveStatementLocator stmtLocator(sourceExtractor, selection);
        MethodExtractorVisitor visitor(stmtLocator);
        visitor.TraverseDecl(ctx.getTranslationUnitDecl());
        return *visitor.getFunctionDecl();
    }
private:
    clang::ASTContext& ctx;
};

void MethodExtractorASTConsumer::HandleTranslationUnit(clang::ASTContext& ctx)
{
    SourceExtractor sourceExtractor(ctx.getSourceManager());
    PrettyFunctionPrinter printer;
    NaiveStatementLocator stmtLocator(sourceExtractor, selection);
    NaiveLocalVariableLocator localVariableLocator;
    DelayedMethodExtractor extractor(sourceExtractor, sourceOperations, printer, localVariableLocator);
    NaiveClangFunctionLocator locator(ctx);
    auto& f = locator.getFunctionContainingSelection(selection);
    auto stmts = stmtLocator.findStatementsInFunction(f);
    extractor.extractStatmentsFromFunctionIntoNewFunction(stmts, f, extractedMethodName);
}

}
