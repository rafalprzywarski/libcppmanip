#ifndef CPPMANIP_5994AB87DE5B483B948FD9AB2776346A_HPP
#define CPPMANIP_5994AB87DE5B483B948FD9AB2776346A_HPP
#include <string>

class TextReplacementListener
{
public:
    virtual ~TextReplacementListener() { }
    virtual void replaceWithTextInRange(const std::string& replacement, unsigned from, unsigned to) = 0;
};

#endif // CPPMANIP_5994AB87DE5B483B948FD9AB2776346A_HPP
