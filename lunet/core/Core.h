#ifndef __CORE__
#define __CORE__

#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <sstream>
#include <functional>
#include "core/Context.h" 
#include "core/MsgQueue.h"
#include "core/Singleton.h"

using Threads = std::vector<std::thread>;
using Task = std::function<void(void)>;
using Tasks = std::vector<Task>;
class Core 
{
public:
    enum eLogLevel 
    {
        eDebug = 0,
        eWarn,
        eInfo,
        eErro,
    };
public:
    Core();
    Core(const Core& c) = delete;
    ~Core();
public:
    void init();
    void run(Tasks&& tasks);
    IContext* NewServer(const std::string& modulename);
    IContext* GetServer(int id);
    bool call(int source, int dest, Msg&& msg);
    void send(int source, int dest, Msg&& msg);
    
public:
    void log(std::stringstream && stream, Core::eLogLevel level);
    
private:
    Threads threads_;
    int incr_;
    eLogLevel loglevel_; 
    bool stop_;
    bool running_;
    std::map<int, IContext*> servers_;
    IContext* logger_;
};

using CoreIns = Singleton<Core>;
#endif //__CORE