#include "Core.h"

Core::Core()
{

}

Core::~Core()
{

}

void Core::run(Tasks&& tasks)
{
    for(auto task : tasks)
    {
        threads_.emplace_back([=]{ while(!stop_) { task(); } });
    }

    for(auto & th : threads_)
    {
        th.join();
    }
}