#ifndef __SINGLETON__
#define __SINGLETON__
#include <thread> 
#include <mutex>

template<typename T>
class Singleton 
{
public:
   Singleton() = delete;
   ~Singleton() = delete; 
public:
    static T* instance();
    static T* init();
private:
    static T* handle_;
    static std::once_flag flag_;
};

template<typename T>
T* Singleton<T>::handle_ = nullptr;

template<typename T>
std::once_flag Singleton<T>::flag_;

template<typename T>
T* Singleton<T>::init()
{
    Singleton<T>::handle_ =  new T();
    return Singleton<T>::handle_;
}

template<typename T>
T* Singleton<T>::instance()
{
    if(Singleton<T>::handle_ == nullptr)
    {
        std::call_once(Singleton<T>::flag_, init);
    }
    return Singleton<T>::handle_;
}
#endif 