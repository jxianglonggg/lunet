#include "Context.h"

Context::Context(ContextFactoryPtr& factoryPtr)
:sid_(0), factoryPtr_(factoryPtr)
{

}

Context::~Context()
{

}

ContextFactory::ContextFactory()
:id_(0)
{

}

ContextFactory::~ContextFactory()
{

}

ContextPtr ContextFactory::get(int sid)
{
    std::lock_guard<std::mutex> lg(lock_);
    for(auto context : contexts_)
    {
        if(context->getid() == sid)
        {
            return context;
        }
    }
    return  ContextPtr(nullptr);
}