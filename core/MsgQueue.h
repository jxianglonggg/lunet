#include <vector>
#include <deque>
#include <thread>
#include <mutex>

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
    Msg(EMSGTYPE type, void* content);
    Msg();
    ~Msg();
    Msg(Msg&& msg);
public:
    bool is(EMSGTYPE type) { return type_ == type; }
    void setid(int id);
    int getid();
public:
    void* getContent();
private:    
    EMSGTYPE type_;
    void* content_;
    int id_;
};

class SubMsgQueue 
{
public:
    SubMsgQueue(const SubMsgQueue& smq) = delete;
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
    ~MsgQueue();
    MsgQueue(MsgQueue&& queue);
public:
    bool fetch(SubMsgQueue& smq);
    int push(int sid, Msg&& msg);
private:
    std::mutex lock_;
    std::deque<SubMsgQueue> subMsgs_;
    int msgid_;
};
