#ifndef __MSG_QUEUE__ 
#define __MSG_QUEUE__
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <string>
#include "any.hpp"
#include "noncopyable.h"
#include "Context.h"

class Msg :noncopyable
{
public:
    enum EMSGTYPE 
    {
        eNone = 0,
        eTest,
        eSocket,
        eText,
    };
    class ContentTest
    {
    public:
        int a;
        int b;
    };
    class ContentText 
    {
    public:
        std::string& getString()
        {
            return s_;
        }
        void setString(std::string& s)
        {
            s_ = s;
        }
    private:
        std::string s_;
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
public:
    int source;
    int dest;
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
#endif //__MSG_QUEUE__
