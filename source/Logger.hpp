#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#define LOG_LINE(text) __FILE__, __LINE__, text

class Logger
{
public:
	enum Level
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal
	};

	Logger();
	Logger(const Logger& other);
	~Logger();

	Logger& operator=(const Logger& other);

	void setLogger(Level l, const std::function<void(const std::string&)>&);

	void log(Level l, const std::string& message, ...) const;
	void log(Level l, const std::string& file, unsigned int line, const std::string& message, ...) const;

private:
	std::unordered_map<Level, std::function<void(const std::string&)>, std::hash<int> > mLoggers;
};
