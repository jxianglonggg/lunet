#ifndef __LIB_HELPER__
#define __LIB_HELPER__
#include <iostream>
#include <map>
#include <functional>
#include <mutex>
#include <dlfcn.h>
#include <stdio.h>
#include "core/Context.h"
#include "core/noncopyable.h"

class LibHelper :noncopyable 
{
public:
    LibHelper()
    {

    }
    ~LibHelper()
    {
        UnloadLibs();
    }
public:
    template<typename T, typename ... Args>
    typename std::result_of<std::function<T>(Args ...)>::type ExcecuteFunc
    (const std::string& libname, const std::string & funcname, Args&& ... args)
    {
        auto f = GetFunction<T>(libname, funcname);
        if(nullptr == f)
        {
            std::string exp = "can con exec function libname=" + libname + " funcname=" + funcname;
            throw std::exception();
        }
        return f(std::forward<Args>(args)...);
    }
private:
    void UnloadLibs()
    {

    }
    template<typename T>
    std::function<T> GetFunction(const std::string& libname, const std::string& funcname)
    {
        std::lock_guard<std::mutex> lg(lock_);
        void* handler = nullptr;
        auto iter = handlers_.find(libname);
        if (iter == handlers_.end())
        {
            std::stringstream stream;
            stream <<  "libs/" << libname << ".so";
            std::string&& fullname = stream.str();
            handler = dlopen(fullname.c_str(), RTLD_LAZY);
            if(nullptr == handler)
            {
                std::cout<<dlerror()<<std::endl;
                return nullptr;
            }
            handlers_.insert(std::make_pair(libname, handler));
        }
        else 
        {
            handler = iter->second;
        }
        
        return std::function<T>((T*)dlsym(handler, funcname.c_str()));
    }
private:
    //using create_func = std::function<IContext*()>;
    std::map<std::string, void*> handlers_;
    std::mutex lock_;
};
#endif //__LIB_HELPER__