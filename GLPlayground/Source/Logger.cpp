#include "Logger.h"
#include <iostream>


Logger::Logger() : LogFileEnable(false)
{
	FileStream.open("log.txt", std::ios::trunc);

	if (FileStream.fail())
	{
		std::cout << "ERROR : File open failed with error : " << strerror(errno) <<std::endl;
	}

	LogString("Started Log sequence", LOG);
}

void Logger::LogString(const std::string & String, LogType Type)
{
	std::string logType;

	switch (Type)
	{
	  case ERROR:
	  {
		 logType = "ERROR -> ";
	  	 break;
	  }
	  case WARNING:
	  {
		 logType = "WARNIG -> ";
	  	 break;
	  }
	  case LOG:
	  {
		  logType = "LOG -> ";
	     break;
	  }
	  
	  default:
	  {
		  logType = "UKNOWN -> ";
		  break;
	  }
	}
	
	std::cout << logType << String << std::endl;

	if (LogFileEnable && FileStream.is_open() && !FileStream.fail())
	{
		FileStream << logType << String << std::endl;
	}
}

Logger::~Logger()
{
	FileStream.close();
}

Logger & Logger::GetLogger()
{
	static Logger Log;

	return Log;
}
