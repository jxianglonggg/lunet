#include <iomanip>
#include <sstream>
#include "Core.h"
#include "libhelper.hpp"
using Libhelper = Singleton<LibHelper>;
using MsgQueueInstance = Singleton<MsgQueue>;
using namespace Lunet;

Core::Core()
:incr_(0),running_(false),loglevel_(Logger::eLogLevel::eDebug)
{
    init();
}

Core::~Core()
{

}

void Core::init()
{
    logger_ = NewServer("logger");
}

void Core::run(Tasks&& tasks)
{
    if(running_)
        return;
    running_ = true;

    for(auto task : tasks)
    {
        threads_.emplace_back([=]{ while(!stop_) { task(); } });
    }

    for(auto & th : threads_)
    {
        th.join();
    }
}

void Core::run(int threadcount)
{
    if(running_)
        return;
    running_ = true;

    auto work_thread = [&]()
    {
        auto mq = MsgQueueInstance::instance();
        SubMsgQueue smq;
        if(mq->fetch(smq))
        {
            ContextPtr server = core->GetServer(smq.getSid());
            if (server == nullptr)
            {

            }
            else 
            {
                for(auto& msg : smq)
                {
                    server->cb(std::move(msg));
                } 
            }
        }
    }
    
    for (auto i = 0;i < threadcount;i++)
    {
        threads_.emplace_back(work_thread);
    }

    for(auto & th : threads_)
    {
        th.join();
    }
}

ContextPtr Core::NewServer(const std::string& modulename)
{
    std::lock_guard<std::mutex> lg(lock_);
    incr_++;
    std::string funcname = "create";
    auto helper = Libhelper::instance();
    ContextPtr context(helper->ExcecuteFunc<IContext*(void)>(modulename, funcname));
    context->SetID(incr_);
    context->init();
    context->SetName(modulename);
    servers_.insert(std::make_pair(incr_, context));
    return context;
}

ContextPtr Core::GetServer(int id)
{
    std::lock_guard<std::mutex> lg(lock_);
    auto iter = servers_.find(id);
    if(iter == servers_.end())
    {
        return nullptr;
    }
    return iter->second;
}

ContextPtr Core::GetServer(const std::string& name)
{
    std::lock_guard<std::mutex> lg(lock_);
    for(auto iter :servers_ )
    {
        if (iter.second->GetName() == name)
        {
            return iter.second;
        }
    }
    return nullptr;
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

void Core::SetLogLevel(Logger::eLogLevel level)
{
    loglevel_ = level;
}

void Core::log(std::stringstream && stream, Logger::eLogLevel level)
{
    if (level >= loglevel_)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tmNow = std::chrono::system_clock::to_time_t(now);
        std::stringstream stime;
        stime << (std::put_time(std::localtime(&tmNow), "[%F %T]"));
        Msg::ContentText content;
        string log = stime.str() + stream.str();
        content.setString(log);
        Msg msg(Msg::EMSGTYPE::eText, content);
        call(0, logger_->getid(), std::move(msg));
    }
}
