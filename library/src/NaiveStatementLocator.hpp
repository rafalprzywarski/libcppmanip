#ifndef CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
#define CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
#include "StatementLocator.hpp"
#include "SourceExtractor.hpp"

class NaiveStatementLocator : public StatementLocator
{
public:
    NaiveStatementLocator(SourceExtractor& sourceExtractor, OffsetRange selection);

    clang::StmtRange findStatementsInFunction(const clang::FunctionDecl& decl);

private:

    SourceExtractor& sourceExtractor;
    OffsetRange selection;

    bool functionDoesNotContainSelection(const clang::FunctionDecl& f);
    clang::StmtRange findStatementsTouchingSelection(const clang::FunctionDecl& func);
    bool selectionOverlapsWithStmt(const clang::Stmt& stmt);
};

#endif // CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
