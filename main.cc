#include <iostream>
#include <random>
#include "core/MsgQueue.h"
#include "core/Core.h"
#include "core/Singleton.h"
using MsgQueueInstance = Singleton<MsgQueue>;
struct product 
{
    int a;
    int b;
};

void test_poduce()
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    product *p = new product();
    p->a = random() % 10;
    p->b = random() % 10;
    Msg msg(Msg::EMSGTYPE::test, (void*)p);
    int msgid = mq->push(1, std::move(msg));
    std::cout<<"produce a="<<p->a<<";b="<<p->b<<";msg id="<< msgid <<std::endl;
}

void test_consume() 
{
    auto mq = MsgQueueInstance::instance();
    //std::cout<<"function="<<__FUNCTION__<<";mq="<<mq<<std::endl;
    product* p = nullptr;
    SubMsgQueue smq;
    if(mq->fetch(smq))
    {
        for(auto& msg : smq)
        {
            if(msg.is(Msg::EMSGTYPE::test))
            {
                p = (product *)msg.getContent();
                std::cout<<"function="<<__FUNCTION__<<";msg id="<<msg.getid()<<";result="<<int(p->a + p->b)<<std::endl;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Core core;

    Tasks tasks;
    tasks.emplace_back(test_poduce);
    tasks.emplace_back(test_poduce);
    tasks.emplace_back(test_consume);
    core.run(std::move(tasks));
    std::cout<<"hello world"<<std::endl;

    return 0;
}