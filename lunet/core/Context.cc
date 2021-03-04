#include "Context.h"

IContext::IContext(ContextFactoryPtr& factoryPtr)
:sid_(0), factoryPtr_(factoryPtr)
{

}

IContext::~IContext()
{

}

int IContext::getid()
{
    return sid_;
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