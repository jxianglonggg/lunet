#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include "any.hpp"
#include "noncopyable.h"

class Msg :noncopyable
{
public:
    enum EMSGTYPE 
    {
        eNone = 0,
        eTest,
        eSocket,
    };
    class ContentTest
    {
    public:
        int a;
        int b;
    };
public:
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

class SubMsgQueue :noncopyable
{
public:
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

class MsgQueue :noncopyable
{
public:
    MsgQueue();
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
