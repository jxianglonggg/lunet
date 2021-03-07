#include "Context.h"
#include "MsgQueue.h"

IContext::IContext()
:sid_(0), name_("")
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

void IContext::SetName(const std::string& name)
{
    name_ = name;
}

const std::string& IContext::GetName()
{
    return name_;
}