#include "CommandLineParser.hpp"
#include <sstream>

ExtractMethodRequest CommandLineParser::parseExtractMethod(int argc, const char** argv) 
{
    ExtractMethodRequest req;
    req.sourceFilename = argv[1];
    req.extractedMethodName = argv[2];
    req.sourceSelection.from = to_u(argv[3]);
    req.sourceSelection.to = to_u(argv[4]);
    return req;
}

unsigned int CommandLineParser::to_u(const char* s) 
{
    std::istringstream ss(s);
    unsigned u;
    ss >> u;
    return u;
}
