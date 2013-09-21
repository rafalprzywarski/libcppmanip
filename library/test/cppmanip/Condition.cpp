#include "Condition.hpp"

namespace cppmanip
{
namespace test
{

Condition::Condition()
    : ready(false) { }

void Condition::notify()
{
    {
        boost::lock_guard<boost::mutex> lock(mutex);
        ready = true;
    }
    cond.notify_one();
}

void Condition::wait()
{
    boost::unique_lock<boost::mutex> lock(mutex);
    while (!ready)
        cond.wait(lock);
}

}
}
