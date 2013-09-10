#ifndef EXTRACTMETHODLISTENER_HPP
#define EXTRACTMETHODLISTENER_HPP

class ExtractMethodListener
{
public:
    virtual ~ExtractMethodListener() { }
    virtual void failed(const std::string& message) = 0;
};

#endif // EXTRACTMETHODLISTENER_HPP
