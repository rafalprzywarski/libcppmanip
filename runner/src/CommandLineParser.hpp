#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP
#include <SourceRange.hpp>
#include <string>
#include <vector>

struct ExtractFunctionRequest
{
    struct Location
    {
        std::string extractedMethodName;
        SourceRange sourceSelection;
    };
    std::string sourceFilename;
    std::vector<Location> locations;
};

class CommandLineParser
{
public:
    ExtractFunctionRequest parseExtractFunction(int argc, const char** argv);
private:
    static unsigned to_u(const char *s);
};

#endif // COMMANDLINEPARSER_HPP
