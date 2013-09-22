#ifndef CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
#define CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
#include "StatementLocator.hpp"
#include "SourceExtractor.hpp"

namespace cppmanip
{

class NaiveStatementLocator : public StatementLocator
{
public:
    NaiveStatementLocator(OffsetRange selection);

    clang::StmtRange findStatementsInFunction(const clang::FunctionDecl& decl);

private:

    OffsetRange selection;

    clang::StmtRange findStatementsTouchingSelection(const clang::FunctionDecl& func);
    bool selectionOverlapsWithStmt(const clang::Stmt& stmt, SourceExtractor& sourceExtractor);
};

}
#endif // CPPMANIP_D889C71C321745A497D7FCEAABF958DC_HPP
