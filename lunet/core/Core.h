#include <iostream>
#include <thread>
#include <vector>
#include <functional>

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
private:
    Threads threads_;
    bool stop_;
};
