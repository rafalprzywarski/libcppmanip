#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <boost/thread.hpp>

class Notifier
{
public:
    virtual ~Notifier() { }
    virtual void notify() = 0;
};

class Waiter
{
public:
    virtual ~Waiter() { }
    virtual void wait() = 0;
};

class Condition : public Notifier, public Waiter, private boost::noncopyable
{
public:
    Condition();
    virtual void notify();
    virtual void wait();
private:
    boost::mutex mutex;
    boost::condition_variable cond;
    bool ready;
};

#endif // CONDITION_HPP
