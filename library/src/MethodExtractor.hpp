#ifndef METHODEXTRACTOR_HPP
#define METHODEXTRACTOR_HPP
#include <clang/AST/RecursiveASTVisitor.h>
#include "SourceExtractor.hpp"
#include "TextOperationApplier.hpp"
#include "FunctionPrinter.hpp"

class MethodExtractor : public clang::RecursiveASTVisitor<MethodExtractor>
{
public:
    MethodExtractor(
        SourceExtractor& sourceExtractor, const std::string& extractedMethodName,
        OffsetRange selection, TextOperationApplier& sourceOperations, FunctionPrinter& functionPrinter);

    bool VisitFunctionDecl(clang::FunctionDecl* decl);

private:
    SourceExtractor& sourceExtractor;
    std::string extractedMethodName;
    OffsetRange selection;
    TextOperationApplier& sourceOperations;
    FunctionPrinter& functionPrinter;

    bool functionDoesNotContainSelection(const clang::FunctionDecl& f);
    void handleFunctionDecl(const clang::FunctionDecl& decl);

    clang::ConstStmtRange findStatementsTouchingSelection(const clang::FunctionDecl& func);
    bool selectionOverlapsWithStmt(const clang::Stmt& stmt);

    void extractStatmentsFromFunction(clang::ConstStmtRange stmts, const clang::FunctionDecl& originalFunction);
    void printExtractedFunction(clang::SourceLocation at, const std::string& name, clang::SourceRange stmtsRange);
    void replaceStatementsWithFunctionCall(clang::SourceRange stmtsRange, const std::string& functionName);
    void replaceRangeWith(clang::SourceRange without, std::string replace);
};

#endif // METHODEXTRACTOR_HPP