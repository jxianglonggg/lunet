#include "core/Context.h"
#include "core/MsgQueue.h"

class LogServer :IContext
{
public:
    bool init() override;
    bool destory() override;
    bool cb(Msg&& msg) override;
private:
    FILE* handle_;
    std::string filename_;
};

extern "C" LogServer* create()
{
    return new LogServer();
}

extern "C" void destory(LogServer* logger)
{
    //logger->destory();
    delete logger;
}