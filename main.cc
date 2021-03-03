#include <iostream>
#include <random>
#include "core/MsgQueue.h"
#include "core/Core.h"
#include "core/Singleton.h"
#include "core/any.hpp"
using MsgQueueInstance = Singleton<MsgQueue>;
struct product 
{
    int a;
    int b;
};

void test_poduce(int svrid)
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    product p;
    p.a = random() % 10;
    p.b = random() % 10;
    Msg msg(Msg::EMSGTYPE::test, p);
    int msgid = mq->push(svrid, std::move(msg));
    std::cout<<"produce to svr="<<svrid<<";a="<<p.a<<";b="<<p.b<<";msg id="<< msgid <<std::endl;
}

void test_consume(int svrid) 
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    product* p = nullptr;
    SubMsgQueue smq;
    if(mq->fetch(svrid, smq))
    {
        for(auto& msg : smq)
        {
            if(msg.is(Msg::EMSGTYPE::test))
            {
                product& p = msg.getContent().AnyCast<product>();
                std::cout<<"consume svrid="<<svrid<<";msg id="<<msg.getid()<<";result="<<int(p.a + p.b)<<std::endl;
            }
        }
    }
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
    //n.AnyCast<int>();
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

int main(int argc, char *argv[])
{
    test_message_queue();
    //test_any();
    std::cout<<"hello world"<<std::endl;

    return 0;
}