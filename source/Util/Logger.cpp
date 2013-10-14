#include "Logger.hpp"
#include "../Application.hpp"

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cstdarg>
#include <ctime>

namespace
{
    std::string getLevelName(Logger::Level l)
    {
        switch(l)
        {
        case 0:
            return "Info";
        case 1:
            return "Debug";
        case 2:
            return "Warning";
        case 3:
            return "Error";
        case 4:
            return "Fatal error";
        case 5:
            return "Script";
        }
        return "Unknown";
    }
    std::string timestamp()
    {
        time_t raw = time(NULL);
        char tmp[256];
        strftime(tmp, 256, "%y-%m-%d %H:%M:%S", localtime(&raw));
        return tmp;
    }
    void defaultLog(const Logger::Message& msg)
    {
        std::ostream& os = (msg.level > 2 ? std::cout : std::cerr);
        os << "[" << timestamp() << "] ";
        if (msg.positional)
            os << msg.file << ":" << msg.line << " ";
        os << getLevelName(msg.level) << ": " << msg.message << std::endl;
    }
}

Logger::Logger(Application& app) : mApp(app), mVerbose(false)
{
    auto& opt = mApp.getOptions();

    opt.registerVariable(mVerbose, "verbose", "Should the program run verbosely");

    for (int i = 2; i < 6; ++i)
        mLoggers[(Level)i] = &defaultLog;
}

Logger::~Logger()
{

}

void Logger::init()
{
    if (mVerbose)
        for (int i = 0; i < 2; ++i)
            mLoggers[(Level)i] = &defaultLog;
    mLoggers[Fatal] = [](const Message& msg) { throw std::runtime_error("FATAL ERROR\n" + msg.message); };

}

void Logger::setLogger(Level l, const std::function<void(const Message&)>& func)
{
    mLoggers[l] = func;
}

void Logger::clearLogger(Level l)
{
    mLoggers.erase(l);
}

void Logger::log(const std::string& file, unsigned int line, const std::string& imsg, Level l, ...) const
{
    if (mLoggers.count(l) == 0)
        return;

    Message m;
    m.level = l;
    m.positional = true;
    m.file = file;
    m.line = line;

    va_list vl;
    va_start(vl, l);

    char tmp[2048];

    vsprintf(tmp, imsg.c_str(), vl);

    m.message = tmp;

    mLoggers.at(l)(m);

    va_end(vl);
}

void Logger::log(const std::string& message, Level l, ...) const
{
    if (mLoggers.count(l) == 0)
        return;

    Message m;
    m.level = l;
    m.line = 0;
    m.positional = false;

    va_list vl;
    va_start(vl, l);

    char tmp[2048];

    vsprintf(tmp, message.c_str(), vl);
    m.message = tmp;

    mLoggers.at(l)(m);

    va_end(vl);
}