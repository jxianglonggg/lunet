#ifndef __CONTEXT__ 
#define __CONTEXT__
#include <memory>
#include <vector>
#include <mutex>
#include "Singleton.h"
#include "noncopyable.h"
#include "Any.hpp"

class Msg;
class IContext :noncopyable
{
public:
    IContext();
    ~IContext();
public:
    virtual bool init() = 0;
    virtual bool destory() = 0;
    virtual bool cb(Msg&& msg) = 0;
public:
    int getid();
    void SetID(int id);
    void SetName(const std::string& name);
    const std::string& GetName(); 
private:
    int sid_;
    std::string name_;
}; 


#endif //__CONTEXT__