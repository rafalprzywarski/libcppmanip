#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP
#include "OffsetRange.hpp"

class CommandLineParser
{
public:
    void parse(int argc, const char** argv);
    std::string getSourceFilename() const { return sourceFilename; }
    std::string getExtractedMethodName() const { return extractedMethodName; }
    OffsetRange getSourceSelection() const { return sourceSelection; }
private:
    std::string sourceFilename;
    std::string extractedMethodName;
    OffsetRange sourceSelection;
    static unsigned to_u(const char *s);
};

#endif // COMMANDLINEPARSER_HPP
