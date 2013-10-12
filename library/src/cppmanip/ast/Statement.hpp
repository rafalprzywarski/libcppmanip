#ifndef CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#define CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#include "LocalVariable.hpp"
#include "SourceOffset.hpp"
#include <memory>
#include <boost/range/iterator_range.hpp>

namespace cppmanip
{
namespace ast
{

class Statement;

typedef std::shared_ptr<const Statement> StatementPtr;
typedef std::vector<StatementPtr> Statements;
typedef std::shared_ptr<const Statements> StatementsPtr;
typedef boost::iterator_range<Statements::const_iterator> StatementRange;

class ScopedStatementRange
{
public:
    ScopedStatementRange(StatementRange scope, StatementRange range)
        : scope(scope), range(range) { }
    StatementRange getScope() const { return scope; }
    StatementRange getRange() const { return range; }
private:
    StatementRange scope, range;
};

class Statement
{
public:
    Statement(const Statement& ) = delete;
    Statement(
        const SourceOffsetRange& range, const LocalVariables& declaredVariables,
        const LocalVariables& usedLocalVariables, const std::string& sourceCode, const std::string& sourceCodeAfter,
        const Statements& children)
        : range(range), declaredVariables(declaredVariables), usedLocalVariables(usedLocalVariables),
        sourceCode(sourceCode), sourceCodeAfter(sourceCodeAfter), children(std::make_shared<Statements>(children)) { }
    SourceOffsetRange getRange() const { return range; }
    const LocalVariables& getDeclaredVariables() const { return declaredVariables; }
    const LocalVariables& getUsedLocalVariables() const { return usedLocalVariables; }
    std::string getSourceCode() const { return sourceCode; }
    std::string getSourceCodeAfter() const { return sourceCodeAfter; }
    const StatementsPtr getChildren() const { return children; }
private:
    const SourceOffsetRange range;
    const LocalVariables declaredVariables;
    const LocalVariables usedLocalVariables;
    const std::string sourceCode, sourceCodeAfter;
    const StatementsPtr children;
};

}
}
#endif // CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
