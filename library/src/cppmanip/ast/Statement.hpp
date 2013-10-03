#ifndef CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#define CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
#include "LocalVariable.hpp"
#include <cppmanip/LocationRange.hpp>
#include <memory>

namespace cppmanip
{
namespace ast
{

class Statement
{
public:
    Statement(const Statement& ) = delete;
    Statement(const LocationRange& range, const LocalVariables& declaredVariables, const LocalVariables& usedLocalVariables)
        : range(range), declaredVariables(declaredVariables), usedLocalVariables(usedLocalVariables) { }
    LocationRange getRange() const { return range; }
    const LocalVariables& getDeclaredVariables() const { return declaredVariables; }
    const LocalVariables& getUsedLocalVariables() const { return usedLocalVariables; }
private:
    const LocationRange range;
    const LocalVariables declaredVariables;
    const LocalVariables usedLocalVariables;
};

typedef std::shared_ptr<const Statement> StatementPtr;
typedef std::vector<StatementPtr> Statements;

}
}
#endif // CPPMANIP_4DC8A8339444434FB393CB6683B50348_HPP
