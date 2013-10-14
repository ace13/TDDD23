#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#define LOG_LINE(text) __FILE__, __LINE__, text

class Application;

class Logger
{
public:
	enum Level
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal,
		Script
	};

    struct Message
    {
        Level level;
        std::string message;

        bool positional;
        std::string file;
        unsigned int line;
    };

	Logger(Application& app);
	~Logger();

	void init();

	void setLogger(Level l, const std::function<void(const Message&)>&);
    void clearLogger(Level l);

	void log(const std::string& message, Level l, ...) const;
	void log(const std::string& file, unsigned int line, const std::string& message, Level l, ...) const;

private:
	std::unordered_map<Level, std::function<void(const Message&)>, std::hash<int> > mLoggers;
	Application& mApp;
	bool mVerbose;
};
