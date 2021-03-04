#include <iostream>
#include "MsgQueue.h"

Msg::Msg()
{
    std::cout<<"Msg create "<<std::endl;
}

Msg::~Msg()
{
    //todo delete
    std::cout<<"Msg release "<<std::endl;
}
Msg::Msg(EMSGTYPE type, Any&& content)
:type_(type), content_(content), id_(0)
{
    std::cout<<"Msg create "<<std::endl;
}
Msg::Msg(Msg&& msg)
{
    type_ = msg.type_;
    content_ = msg.content_;
    id_ = msg.id_;
    msg.content_ = nullptr;
    msg.type_ = EMSGTYPE::eNone;
    msg.id_ = 0;
}

void Msg::setid(int id)
{
    id_ = id;
}

int Msg::getid()
{
    return id_;
}

Any& Msg::getContent()
{
    return content_;
}

SubMsgQueue::SubMsgQueue()
:sid_(0)
{
    std::cout<<"SubMsgQueue create "<<std::endl;
}
SubMsgQueue::SubMsgQueue(int sid, Msg&& msg)
:sid_(sid)
{
    msgs_.emplace_back(std::move(msg));
}

SubMsgQueue::~SubMsgQueue()
{
    std::cout<<"SubMsgQueue release "<<std::endl;
}

SubMsgQueue::SubMsgQueue(SubMsgQueue&& smq)
{
    {
        std::lock_guard<std::mutex> lg(smq.lock_);
        msgs_ = std::move(smq.msgs_);
    }

    sid_ = smq.sid_;
    smq.sid_ = 0;
}

void SubMsgQueue::setSid(int sid)
{
    sid_ = sid;
}

int SubMsgQueue::getSid()
{
    return sid_;
}

SubMsgQueue& SubMsgQueue::operator=(SubMsgQueue&& smq)
{
    std::lock_guard<std::mutex> lg(lock_);
    msgs_ = std::move(smq.msgs_);
    sid_ = smq.sid_;
    smq.sid_ = 0;
    return *this;
}

void SubMsgQueue::push(Msg&& msg)
{
    std::lock_guard<std::mutex> lg(lock_);
    msgs_.emplace_back(std::move(msg));
    std::cout<<"sid_="<<sid_<<";size="<<msgs_.size()<<std::endl;
}

MsgQueue::MsgQueue()
:msgid_(0)
{
    std::cout<<"MsgQueue create "<<std::endl;
}

MsgQueue::~MsgQueue()
{
    std::cout<<"MsgQueue release "<<std::endl;
}

MsgQueue::MsgQueue(MsgQueue&& mq)
{
    std::lock_guard<std::mutex> lg(mq.lock_);
    subMsgs_ = std::move(mq.subMsgs_);
}

bool MsgQueue::fetch(SubMsgQueue& smq)
{
    std::lock_guard<std::mutex> lg(lock_);
    if(subMsgs_.size() > 0)
    {
        smq = std::move(subMsgs_.front());
        subMsgs_.pop_front();
        return true;
    }
    return false;
}

bool MsgQueue::fetch(int sid, SubMsgQueue& smqout)
{
    std::lock_guard<std::mutex> lg(lock_);
    auto iter = subMsgs_.begin();
    for(;iter != subMsgs_.end();iter++)
    {
        if (iter->getSid() == sid)
        {
            smqout = std::move(*iter);
            subMsgs_.erase(iter);
            return true;
        }
    }
    return false;
}

int MsgQueue::push(int sid, Msg&& msg)
{
    std::lock_guard<std::mutex> lg(lock_);
    msgid_++;
    msg.setid(msgid_);
    bool ok = false;
    for(auto& smq : subMsgs_)
    {
        if(sid == smq.getSid())
        {
            smq.push(std::move(msg));
            ok = true;
            break;
        }
    }
    if(!ok)
    {
        subMsgs_.emplace_back(sid, std::move(msg));
    }
    std::cout<<"total queue size="<<subMsgs_.size()<<std::endl;
    return msgid_;
}