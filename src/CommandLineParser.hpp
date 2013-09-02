#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP
#include "OffsetRange.hpp"

struct UserSelection
{
    unsigned from, to;
    
    UserSelection() : from(0), to(0) { }
};

struct OperationRequest
{
    std::string sourceFilename;
    std::string extractedMethodName;
    UserSelection sourceSelection;
};

class CommandLineParser
{
public:
    OperationRequest parse(int argc, const char** argv);
private:
    static unsigned to_u(const char *s);
};

#endif // COMMANDLINEPARSER_HPP
