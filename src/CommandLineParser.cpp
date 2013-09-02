#include "CommandLineParser.hpp"
#include <sstream>

OperationRequest CommandLineParser::parse(int argc, const char** argv) 
{
    OperationRequest req;
    req.sourceFilename = argv[1];
    req.extractedMethodName = argv[3];
    req.sourceSelection.from = to_u(argv[4]);
    req.sourceSelection.to = to_u(argv[5]);
    return req;
}

unsigned int CommandLineParser::to_u(const char* s) 
{
    std::istringstream ss(s);
    unsigned u;
    ss >> u;
    return u;
}
