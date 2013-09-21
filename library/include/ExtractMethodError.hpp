#include <stdexcept>

class ExtractMethodError : public std::runtime_error
{
public:
    explicit ExtractMethodError(const std::string& msg) : std::runtime_error(msg) { }
};