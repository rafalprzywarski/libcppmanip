#include "CommandLineParser.hpp"
#include <CppManip.hpp>
#include <stdexcept>
#include <iostream>

void performOperation(const OperationRequest& req)
{
    extractMethodInFile(req.extractedMethodName, req.sourceSelection, req.sourceFilename);
}

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto operationRequest = parser.parse(argc, argv);
        performOperation(operationRequest);
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown internal error" << std::endl;
        return 1;
    }
}