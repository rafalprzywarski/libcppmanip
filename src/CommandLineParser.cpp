#include "CommandLineParser.hpp"
#include <sstream>

void CommandLineParser::parse(int argc, const char** argv) 
{
    sourceFilename = argv[1];
    extractedMethodName = argv[3];
    sourceSelection = OffsetRange(to_u(argv[4]), to_u(argv[5]));
}

unsigned int CommandLineParser::to_u(const char* s) 
{
    std::istringstream ss(s);
    unsigned u;
    ss >> u;
    return u;
}
