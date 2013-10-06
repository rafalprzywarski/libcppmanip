#ifndef CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#define CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#include "LocalVariable.hpp"
#include "SourceOffset.hpp"
#include <memory>
#include <boost/range/iterator_range.hpp>
#include <clang/AST/Stmt.h>

namespace clang { class Stmt; }

namespace cppmanip
{
namespace ast
{

class Statement
{
public:
    Statement(const Statement& ) = delete;
    Statement(
        clang::Stmt *stmt, const SourceOffsetRange& range, const LocalVariables& declaredVariables,
        const LocalVariables& usedLocalVariables, const std::string& sourceCode, const std::string& sourceCodeAfter)
        : stmt(stmt), range(range), declaredVariables(declaredVariables), usedLocalVariables(usedLocalVariables),
        sourceCode(sourceCode), sourceCodeAfter(sourceCodeAfter) { }
    SourceOffsetRange getRange() const { return range; }
    const LocalVariables& getDeclaredVariables() const { return declaredVariables; }
    const LocalVariables& getUsedLocalVariables() const { return usedLocalVariables; }
    std::string getSourceCode() const { return sourceCode; }
    std::string getSourceCodeAfter() const { return sourceCodeAfter; }
    clang::Stmt *getStmt() const { return stmt; }
private:
    clang::Stmt *const stmt; // TODO: temporarily
    const SourceOffsetRange range;
    const LocalVariables declaredVariables;
    const LocalVariables usedLocalVariables;
    const std::string sourceCode, sourceCodeAfter;
};

typedef std::shared_ptr<const Statement> StatementPtr;
typedef std::vector<StatementPtr> Statements;
typedef boost::iterator_range<Statements::const_iterator> StatementRange;


}
}
#endif // CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
