#ifndef CPPMANIP_E91CDB7434A74B7A882950BFC7B5B2B6_HPP
#define CPPMANIP_E91CDB7434A74B7A882950BFC7B5B2B6_HPP
#include <memory>
#include "Function.hpp"

namespace cppmanip
{
namespace ast
{

class Gateway
{
public:
    virtual ~Gateway() { }
    virtual ast::FunctionPtr getFunctionInSelectionFromFile(ast::SourceOffsetRange selection, const std::string& filename) = 0;
};

typedef std::shared_ptr<Gateway> GatewayPtr;

}
}
#endif // CPPMANIP_E91CDB7434A74B7A882950BFC7B5B2B6_HPP
