#ifndef __CONTEXT__ 
#define __CONTEXT__
#include <memory>
#include <vector>
#include <mutex>
#include "Singleton.h"
#include "noncopyable.h"

class ContextFactory ;
using ContextFactorySingleton = Singleton<ContextFactory>;
using ContextFactoryPtr = std::weak_ptr<ContextFactory>;
class IContext :noncopyable
{
public:
    IContext(ContextFactoryPtr& factoryPtr);
    ~IContext();
public:
    int getid();
private:
    int sid_;
    ContextFactoryPtr factoryPtr_;
};

using ContextPtr = std::shared_ptr<IContext>;
using ContextPtrs = std::vector<ContextPtr>;
class ContextFactory 
{
public:
    ContextFactory(const ContextFactory& cf) = delete;
    ContextFactory();
    ~ContextFactory();
public:
    //ContextPtr create();
    ContextPtr get(int sid);
private:
    ContextPtrs contexts_;
    int id_;
    std::mutex lock_;
};
#endif //__CONTEXT__