#ifndef CPPMANIP_4C010B1504FA456091DA9EC210C6128D_HPP
#define CPPMANIP_4C010B1504FA456091DA9EC210C6128D_HPP

#include <boost/thread.hpp>

namespace cppmanip
{
namespace test
{

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

}
}
#endif // CPPMANIP_4C010B1504FA456091DA9EC210C6128D_HPP
