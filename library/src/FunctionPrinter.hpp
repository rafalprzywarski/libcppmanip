#ifndef FUNCTIONPRINTER_HPP
#define FUNCTIONPRINTER_HPP
#include <string>
#include <vector>

class FunctionPrinter
{
public:
    typedef std::vector<std::string> Strings;
    virtual ~FunctionPrinter() { }
    virtual std::string printFunction(const std::string& name, const Strings& args, const std::string& body) = 0;
    virtual std::string printFunctionCall(const std::string& name, const Strings& args) = 0;
};

#endif // FUNCTIONPRINTER_HPP
