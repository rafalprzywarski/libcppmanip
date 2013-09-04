#include "MethodExtractor.hpp"

MethodExtractor::MethodExtractor(
    SourceExtractor& sourceExtractor, const std::string& extractedMethodName,
    OffsetRange selection, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter)
    : sourceExtractor(sourceExtractor), extractedMethodName(extractedMethodName),
    selection(selection), sourceOperations(sourceOperations), functionPrinter(functionPrinter) { }

bool MethodExtractor::VisitFunctionDecl(clang::FunctionDecl* decl)
{
    handleFunctionDecl(*decl);
    return true;
}

bool MethodExtractor::functionDoesNotContainSelection(const clang::FunctionDecl& f)
{
    return !sourceExtractor.isLocationFromMainFile(f.getLocation()) || !f.hasBody();
}

void MethodExtractor::handleFunctionDecl(const clang::FunctionDecl& decl)
{
    if (functionDoesNotContainSelection(decl))
        return;
    auto stmts = findStatementsTouchingSelection(decl);
    extractStatmentsFromFunction(stmts, decl);
}

void MethodExtractor::extractStatmentsFromFunction(clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction)
{
    auto stmtsRange = sourceExtractor.getCorrectSourceRange(stmts);
    auto originalFunctionLocation = sourceExtractor.getCorrectSourceRange(originalFunction).getBegin();
    printExtractedFunction(originalFunctionLocation, extractedMethodName, stmtsRange);
    replaceStatementsWithFunctionCall(stmtsRange, extractedMethodName);
}

bool MethodExtractor::selectionOverlapsWithStmt(const clang::Stmt& stmt)
{
    return selection.overlapsWith(sourceExtractor.getOffsetRange(sourceExtractor.getCorrectSourceRange(stmt)));
}

clang::ConstStmtRange MethodExtractor::findStatementsTouchingSelection(const clang::FunctionDecl& func)
{
    auto body = func.getBody();
    auto begin =
        std::find_if(body->child_begin(), body->child_end(), [&](clang::Stmt *s) { return selectionOverlapsWithStmt(*s); });
    auto end =
        std::find_if(begin, body->child_end(), [&](clang::Stmt *s) { return !selectionOverlapsWithStmt(*s); });
    return {begin, end};
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
