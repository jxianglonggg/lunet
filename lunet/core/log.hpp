#ifndef __LOG__
#define __LOG__
#include "core/Core.h"

void log(std::stringstream& stream, Core::eLogLevel level)
{
    CoreIns::instance()->log(std::move(stream), level);
}

template <typename T, typename ... Args>
void log(std::stringstream& stream, Core::eLogLevel level, T head, Args ... args)
{
    stream << head;
    log(stream, level, args...);
}

const char* logI2S[] = {"D", "W", "I", "E"};
template <typename ... Args>
void log(Core::eLogLevel level, const char* file, const int line, Args ... args)
{
    char debugstr[128] = { 0 };
    snprintf(debugstr, sizeof(debugstr), "[%s.%d][%s]", file, line, logI2S[level]);
    std::stringstream stream;
    stream << debugstr;
    log(stream, level, args...);  
}

template <typename ... Args>
void logd(Args ... args)
{
    log(Core::eDebug, args...);
}

template <typename ... Args>
void logw(Args ... args)
{
    log(Core::eWarn, args...);
}

template <typename ... Args>
void logi(Args ... args)
{
    log(Core::eInfo, args...);
}

template <typename ... Args>
void loge(Args ... args)
{
    log(Core::eErro, args...);
}

#define LOGD(...) logd(__FILE__, __LINE__, __VA_ARGS__)
#define LOGW(...) logw(__FILE__, __LINE__, __VA_ARGS__)
#define LOGI(...) logi(__FILE__, __LINE__, __VA_ARGS__)
#define LOGE(...) logE(__FILE__, __LINE__, __VA_ARGS__)
#endif //__LOG__