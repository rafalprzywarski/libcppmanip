#ifndef CPPMANIP_351FD439812D4CC0897D471644909E17_HPP
#define CPPMANIP_351FD439812D4CC0897D471644909E17_HPP
#include "ast/Function.hpp"

namespace cppmanip
{

class FunctionExtractionValidator
{
public:
    virtual ~FunctionExtractionValidator() { }
    virtual void validateStatements(const std::string& functionName, ast::ScopedStatementRange selected) = 0;
};

typedef std::shared_ptr<FunctionExtractionValidator> FunctionExtractionValidatorPtr;

}
#endif // CPPMANIP_351FD439812D4CC0897D471644909E17_HPP
