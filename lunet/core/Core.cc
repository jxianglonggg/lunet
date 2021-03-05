#include "Core.h"
#include "libhelper.hpp"
using Libhelper = Singleton<LibHelper>;
using MsgQueueInstance = Singleton<MsgQueue>;

Core::Core()
:incr_(0)
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

IContext* Core::NewServer(const std::string& modulename)
{
    incr_++;
    std::string funcname = "create";
    auto helper = Libhelper::instance();
    IContext* context  = helper->ExcecuteFunc<IContext*(void)>(modulename, funcname);
    context->SetID(incr_);
    context->init();
    servers_.insert(std::make_pair(incr_, context));
    return context;
}

IContext* Core::GetServer(int id)
{
    auto iter = servers_.find(id);
    if(iter == servers_.end())
    {
        return nullptr;
    }
    return iter->second;
}

bool Core::call(int source, int dest, Msg&& msg)
{
    msg.source = source;
    msg.dest = dest;
    auto MsgQueue = MsgQueueInstance::instance();
    MsgQueue->push(dest, std::move(msg));
    return true;
}

void Core::send(int source, int dest, Msg&& msg)
{

}