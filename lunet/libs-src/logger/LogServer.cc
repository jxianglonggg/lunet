#include <sstream>
#include <iomanip>
#include "LogServer.h"

bool LogServer::init()
{
    auto now = std::chrono::system_clock::now();
    std::time_t tmNow = std::chrono::system_clock::to_time_t(now);
    std::stringstream stream;
    stream << std::put_time(std::localtime(&tmNow), "%G%m%d_%H%M%I");
    filename_ = "lunet_" + stream.str() + ".log";
    handle_ = ::fopen(filename_.c_str(), "a");
    return true;
}

bool LogServer::destory()
{
    ::fclose(handle_);
    return true;
}
bool LogServer::cb(Msg&& msg)
{
    Msg::ContentText content = msg.getContent().AnyCast<Msg::ContentText>();
    std::string& s = content.getString();
	fwrite(s.c_str(), s.length() , 1, handle_);
	fprintf(handle_, "\n");
    fflush(handle_);
    return true;
}
