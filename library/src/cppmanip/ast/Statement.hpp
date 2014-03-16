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
typedef std::vector<StatementsPtr> StatementGroups;

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
        const SourceOffsetRange& range, const SourceOffsetRanges& specificRanges, const LocalVariables& declaredVariables,
        const LocalVariables& usedLocalVariables, const std::string& sourceCode, const std::string& sourceCodeAfter,
        const StatementGroups& children)
        : range(range), specificRanges(specificRanges), declaredVariables(declaredVariables), usedLocalVariables(usedLocalVariables),
        sourceCode(sourceCode), sourceCodeAfter(sourceCodeAfter), children(children) { }
    SourceOffsetRange getRange() const { return range; }
    SourceOffsetRanges getSpecificRanges() const { return specificRanges; }
    const LocalVariables& getDeclaredVariables() const { return declaredVariables; }
    const LocalVariables& getUsedLocalVariables() const { return usedLocalVariables; }
    std::string getSourceCode() const { return sourceCode; }
    std::string getSourceCodeAfter() const { return sourceCodeAfter; }
    const StatementGroups getChildGroups() const { return children; }
private:
    const SourceOffsetRange range;
    const SourceOffsetRanges specificRanges;
    const LocalVariables declaredVariables;
    const LocalVariables usedLocalVariables;
    const std::string sourceCode, sourceCodeAfter;
    const StatementGroups children;
};

}
}
#endif // CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
