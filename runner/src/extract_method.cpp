#include "CommandLineParser.hpp"
#include <CppManip.hpp>
#include <ExtractMethodListener.hpp>
#include <stdexcept>
#include <iostream>

class ErrorListener : public ExtractMethodListener
{
public:
    void failed(const std::string& message)
    {
        extractionFailed = true;
        this->message = message;
    }
    std::string getMessage() const { return message; }
    bool hasExtractionFailed() const { return extractionFailed; }
private:
    bool extractionFailed = false;
    std::string message;
};

int main(int argc, const char** argv)
{
    try
    {
        CommandLineParser parser;
        auto req = parser.parseExtractMethod(argc, argv);
        ErrorListener errorListener;
        extractMethodInFile(req.extractedMethodName, req.sourceSelection, req.sourceFilename, errorListener);
        if (errorListener.hasExtractionFailed())
        {
            std::cerr << errorListener.getMessage() << std::endl;
            return 1;
        }
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