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

class Statement
{
public:
    Statement(const Statement& ) = delete;
    Statement(const SourceOffsetRange& range, const LocalVariables& declaredVariables, const LocalVariables& usedLocalVariables)
        : range(range), declaredVariables(declaredVariables), usedLocalVariables(usedLocalVariables) { }
    SourceOffsetRange getRange() const { return range; }
    const LocalVariables& getDeclaredVariables() const { return declaredVariables; }
    const LocalVariables& getUsedLocalVariables() const { return usedLocalVariables; }
private:
    const SourceOffsetRange range;
    const LocalVariables declaredVariables;
    const LocalVariables usedLocalVariables;
};

typedef std::shared_ptr<const Statement> StatementPtr;
typedef std::vector<StatementPtr> Statements;
typedef boost::iterator_range<Statements::const_iterator> StatementRange;


}
}
#endif // CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
