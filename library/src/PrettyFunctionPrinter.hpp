#ifndef PRETTYFUNCTIONPRINTER_HPP
#define PRETTYFUNCTIONPRINTER_HPP
#include "FunctionPrinter.hpp"

class PrettyFunctionPrinter : public FunctionPrinter
{
public:
    virtual std::string printFunction(const std::string& name, const Strings& args, const std::string& body);
    virtual std::string printFunctionCall(const std::string& name, const Strings& args);
};

#endif // PRETTYFUNCTIONPRINTER_HPP
