#include "Logger.hpp"

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cstdarg>

Logger::Logger()
{
	mLoggers[Warning] = [](const std::string& msg) { std::cerr << msg << std::endl; };
	mLoggers[Error] = [](const std::string& msg) { std::cout << msg << std::endl; };
	mLoggers[Fatal] = [](const std::string& msg) { std::cout << msg << std::endl; throw std::runtime_error(msg); };
}

Logger::Logger(const Logger& other)
{
	mLoggers.clear();

	for (auto it = other.mLoggers.begin(); it != other.mLoggers.end(); ++it)
	{
		mLoggers[it->first] = it->second;
	}
}

Logger::~Logger()
{

}

Logger& Logger::operator=(const Logger& other)
{
	if (this == &other)
		return *this;

	mLoggers.clear();

	for (auto it = other.mLoggers.begin(); it != other.mLoggers.end(); ++it)
	{
		mLoggers[it->first] = it->second;
	}

	return *this;
}

void Logger::setLogger(Level l, const std::function<void(const std::string&)>& func)
{
	mLoggers[l] = func;
}

void Logger::log(Level l, const std::string& file, unsigned int line, const std::string& imsg, ...) const
{
	if (mLoggers.count(l) == 0)
		return;

	va_list vl;
	va_start(vl, imsg);

	char tmp[2048];

	vsprintf(tmp, imsg.c_str(), vl);

	std::string msg = tmp;

	auto name = [l]() -> std::string {
		switch(l)
		{
		case Info:
			return "Info";
		case Debug:
			return "Debug";
		case Warning:
			return "Warning";
		case Error:
			return "Error";
		case Fatal:
			return "Fatal error";
		}
		return "Unknown";
	};

	sprintf(tmp, "%s:%d: %s: %s", file.c_str(), line, name().c_str(), msg.c_str());

	mLoggers.at(l)(tmp);

	va_end(vl);
}

void Logger::log(Level l, const std::string& message, ...) const
{
	if (mLoggers.count(l) == 0)
		return;

	va_list vl;
	va_start(vl, message);

	char tmp[2048];

	auto name = [l]() -> std::string {
		switch(l)
		{
		case Info:
			return "Info";
		case Debug:
			return "Debug";
		case Warning:
			return "Warning";
		case Error:
			return "Error";
		case Fatal:
			return "Fatal error";
		}
		return "Unknown";
	};

	vsprintf(tmp, message.c_str(), vl);
	std::string msg = tmp;
	sprintf(tmp, "%s: %s", name().c_str(), msg.c_str());

	mLoggers.at(l)(tmp);

	va_end(vl);
}