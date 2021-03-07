#ifndef __CORE__
#define __CORE__

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include <mutex>
#include "core/define.h"
#include "core/noncopyable.h"
#include "core/Context.h" 
#include "core/MsgQueue.h"
#include "core/Singleton.h"

using Threads = std::vector<std::thread>;
using Task = std::function<void(void)>;
using Tasks = std::vector<Task>;
namespace Lunet{
class Core :noncopyable
{
public:
    Core();
    Core(const Core& c) = delete;
    ~Core();
public:
    void init();
    void run(Tasks&& tasks);
    ContextPtr NewServer(const std::string& modulename);
    ContextPtr GetServer(int id);
    ContextPtr GetServer(const std::string& name);
    bool call(int source, int dest, Msg&& msg);
    void send(int source, int dest, Msg&& msg);
public:
    void SetLogLevel(Logger::eLogLevel level);
    void log(std::stringstream && stream, Logger::eLogLevel level);
    
private:
    std::mutex lock_;
    Threads threads_;
    int incr_;
    Logger::eLogLevel loglevel_; 
    bool stop_;
    bool running_;
    std::map<int, ContextPtr> servers_;
    ContextPtr logger_;
}; //Core
}; //Lunet


using CoreIns = Singleton<Lunet::Core>;
#endif //__CORE__