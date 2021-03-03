#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include "any.hpp"

class Msg 
{
public:
    enum EMSGTYPE 
    {
        none = 0,
        test,
        socket,
    };
public:
    Msg(const Msg& msg) = delete;
    Msg& operator = (const Msg& msg) = delete;
    Msg(EMSGTYPE type, Any&& content);
    Msg();
    ~Msg();
    Msg(Msg&& msg);
public:
    bool is(EMSGTYPE type) { return type_ == type; }
    void setid(int id);
    int getid();
public:
    Any& getContent();
private:    
    EMSGTYPE type_;
    Any content_;
    int id_;
};

class SubMsgQueue 
{
public:
    SubMsgQueue(const SubMsgQueue& smq) = delete;
    SubMsgQueue& operator = (const SubMsgQueue& smq) = delete;
    SubMsgQueue();
    SubMsgQueue(int sid, Msg&& msg);
    ~SubMsgQueue();
    SubMsgQueue(SubMsgQueue&& smq);
public:
    SubMsgQueue& operator=(SubMsgQueue&& smq);
public:
    std::vector<Msg>::iterator begin() { return msgs_.begin(); }
    std::vector<Msg>::iterator end() { return msgs_.end(); }
public:
    void setSid(int sid);
    int getSid();
    void push(Msg&& msg);

private:
    std::mutex lock_;
    std::vector<Msg> msgs_;
    int sid_;
};

class MsgQueue
{
public:
    MsgQueue();
    MsgQueue(const MsgQueue& mq)= delete;
    MsgQueue& operator = (const MsgQueue& mq) = delete;
    ~MsgQueue();
    MsgQueue(MsgQueue&& queue);
public:
    bool fetch(SubMsgQueue& smq);
    bool fetch(int sid, SubMsgQueue& smq);
    int push(int sid, Msg&& msg);
private:
    std::mutex lock_;
    std::deque<SubMsgQueue> subMsgs_;
    int msgid_;
};
