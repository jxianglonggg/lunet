#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <functional>
#include "core/Context.h" 
#include "core/MsgQueue.h"

using Threads = std::vector<std::thread>;
using Task = std::function<void(void)>;
using Tasks = std::vector<Task>;
class Core 
{
public:
    Core();
    Core(const Core& c) = delete;
    ~Core();
public:
    void run(Tasks&& tasks);
    IContext* NewServer(const std::string& modulename);
    IContext* GetServer(int id);
    bool call(int source, int dest, Msg&& msg);
    void send(int source, int dest, Msg&& msg);
private:
    Threads threads_;
    int incr_;
    bool stop_;
    std::map<int, IContext*> servers_;
};
