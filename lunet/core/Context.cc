#include "Context.h"
#include "MsgQueue.h"

IContext::IContext()
:sid_(0)
{

}

IContext::~IContext()
{

}

int IContext::getid()
{
    return sid_;
}

void IContext::SetID(int id)
{
    sid_ = id;
}
