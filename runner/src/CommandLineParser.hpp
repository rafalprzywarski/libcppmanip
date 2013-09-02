#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP
#include <SourceSelection.hpp>
#include <string>

struct OperationRequest
{
    std::string sourceFilename;
    std::string extractedMethodName;
    SourceSelection sourceSelection;
};

class CommandLineParser
{
public:
    OperationRequest parse(int argc, const char** argv);
private:
    static unsigned to_u(const char *s);
};

#endif // COMMANDLINEPARSER_HPP
