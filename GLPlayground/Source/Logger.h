#pragma once
#include <string>
#include <fstream>

enum LogType
{
	ERROR,
	WARNING,
	LOG,
};

class Logger
{
public:
	Logger();
	void LogString(const std::string & String, LogType Type);
	~Logger();

	void LogOnFile(bool LogEnable)
	{
		LogFileEnable = LogEnable;
	}

	static Logger & GetLogger();
private:
	std::ofstream FileStream;
	bool LogFileEnable;
};


