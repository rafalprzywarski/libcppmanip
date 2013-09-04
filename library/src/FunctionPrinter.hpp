#ifndef FUNCTIONPRINTER_HPP
#define FUNCTIONPRINTER_HPP
#include <string>

class FunctionPrinter
{
public:
    virtual ~FunctionPrinter() { }
    virtual std::string printFunction(const std::string& name, const std::string& body) = 0;
    virtual std::string printFunctionCall(const std::string& name) = 0;
};

#endif // FUNCTIONPRINTER_HPP
