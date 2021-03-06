#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include "core/MsgQueue.h"
#include "core/Singleton.h"
#include "core/any.hpp"
#include "core/Context.h"
#include "core/libhelper.hpp"
#include "core/Core.h"
#include "core/log.hpp"

using MsgQueueInstance = Singleton<MsgQueue>;
using Libhelper = Singleton<LibHelper>;

void test_poduce(int svrid)
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    Msg::ContentTest content;
    content.a = random() % 10;
    content.b = random() % 10;
    Msg msg(Msg::EMSGTYPE::eTest, content);
    int msgid = mq->push(svrid, std::move(msg));
    std::cout<<"produce to svr="<<svrid<<";a="<<content.a<<";b="<<content.b<<";msg id="<< msgid <<std::endl;
    auto stime = chrono::seconds(random() % 5);
    std::this_thread::sleep_for(stime);
}

void test_consume(int svrid) 
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    SubMsgQueue smq;
    if(mq->fetch(svrid, smq))
    {
        for(auto& msg : smq)
        {
            if(msg.is(Msg::EMSGTYPE::eTest))
            {
                Msg::ContentTest content = msg.getContent().AnyCast<Msg::ContentTest>();
                std::cout<<"consume svrid="<<svrid<<";msg id="<<msg.getid()<<";result="<<int(content.a + content.b)<<std::endl;
            }
        }
    }
    auto stime = chrono::seconds(random() % 10);
    std::this_thread::sleep_for(stime);
}

void test_message_queue()
{
    Core* core = new Core();

    Tasks tasks;
    tasks.emplace_back(std::bind(test_poduce, 1));
    tasks.emplace_back(std::bind(test_poduce, 1));
    tasks.emplace_back(std::bind(test_poduce, 2));
    tasks.emplace_back(std::bind(test_consume, 1));
    tasks.emplace_back(std::bind(test_consume, 2));
    core->run(std::move(tasks));
}

void test_any()
{
    Any n;
    auto r = n.IsNull();
    string s = "hello";
    n = s;
    Any n1= 1;
    n1.Is<int>();
    Any a = 1;
    Any b = "b";
    std::vector<Any> anys;
    anys.push_back(a);
    anys.push_back(b);
    anys.push_back(s);
    for(auto& v : anys)
    {
        if(v.Is<int>())
        {
            std::cout<<"v="<<v.AnyCast<int>()<<std::endl;
        }
        else if(v.Is<const char*>())
        {
            std::cout<<"v="<<v.AnyCast<const char*>()<<std::endl;
        }
        else 
        {
            std::cout<<"v="<<v.AnyCast<string>()<<std::endl;
        }
    }
}

void test_logger()
{
    std::shared_ptr<Core> core(CoreIns::instance());

    auto produce_log = [&]
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tmNow = std::chrono::system_clock::to_time_t(now);
        std::stringstream stream;
        stream << std::put_time(std::localtime(&tmNow), "%F %T");
        LOGD(1, "a", 2, "d ddd", 100);
    };

    auto work_thread = [&]
    {
        auto mq = MsgQueueInstance::instance();
        SubMsgQueue smq;
        if(mq->fetch(smq))
        {
            IContext* server = core->GetServer(smq.getSid());
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
    };
    Tasks tasks;
    tasks.emplace_back(std::bind(produce_log));
    tasks.emplace_back(std::bind(produce_log));
    tasks.emplace_back(std::bind(produce_log));
    tasks.emplace_back(std::bind(work_thread));
    core->run(std::move(tasks));
}

int main(int argc, char *argv[])
{
    //test_message_queue();
    //test_any();
    test_logger();
    std::cout<<"hello world"<<std::endl;

    return 0;
}