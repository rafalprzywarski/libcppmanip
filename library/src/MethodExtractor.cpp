#include "MethodExtractor.hpp"

MethodExtractor::MethodExtractor(
    SourceExtractor& sourceExtractor, const std::string& extractedMethodName,
    StatementLocator& stmtLocator, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter)
    : sourceExtractor(sourceExtractor), extractedMethodName(extractedMethodName),
    stmtLocator(stmtLocator), sourceOperations(sourceOperations), functionPrinter(functionPrinter) { }

bool MethodExtractor::VisitFunctionDecl(clang::FunctionDecl* decl)
{
    handleFunctionDecl(*decl);
    return true;
}

void MethodExtractor::handleFunctionDecl(const clang::FunctionDecl& decl)
{
    auto stmts = stmtLocator.findStatementsInFunction(decl);
    if (!stmts)
        return;
    extractStatmentsFromFunction(stmts, decl);
}

void MethodExtractor::extractStatmentsFromFunction(clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
    auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    printExtractedFunction(originalFunctionLocation, extractedMethodName, stmtsRange);
    replaceStatementsWithFunctionCall(stmtsRange, extractedMethodName);
}

void MethodExtractor::printExtractedFunction(clang::SourceLocation at, const std::string& name, clang::SourceRange stmtsRange)
{
    auto source = functionPrinter.printFunction(name, sourceExtractor.getSource(stmtsRange));
    sourceOperations.insertTextAt(source, sourceExtractor.getOffset(at));
}

void MethodExtractor::replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName)
{
    replaceRangeWith(stmtsRange, functionPrinter.printFunctionCall(functionName));
}

void MethodExtractor::replaceRangeWith(clang::SourceRange without, std::string replace)
{
    auto begin = sourceExtractor.getOffset(without.getBegin());
    auto end = sourceExtractor.getOffset(without.getEnd());
    sourceOperations.removeTextInRange(begin, end);
    sourceOperations.insertTextAt(replace, begin);
}
