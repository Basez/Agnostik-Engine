#include "asharedh.hpp"
#include "alogger_linux.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <sys/io.h>
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include <iomanip>
#include <cstdarg>

AGN::ALoggerLinux loggerLinux = AGN::ALoggerLinux();
AGN::IALogger& g_log = loggerLinux;

void AGN::ALoggerLinux::init(EALogTimeType a_timeType, uint8_t a_outputTypes)
{
	m_outputTypes = a_outputTypes;
	m_timeType = a_timeType;

	m_startTickCount = (float)getTickCount();
}

void AGN::ALoggerLinux::info(const char *a_info, ...)
{
	// Use the formatting to get the correct log
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog);
	sprintf(messageLog, "\033[36m%s| %s\033[0m\n", timeLog, message); // blue-aquaish
	
	log(messageLog);
}

void AGN::ALoggerLinux::debug(const char *a_info, ...)
{
	// Use the formatting to get the correct log
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog);
	sprintf(messageLog, "\033[32m%s| %s\033[0m\n", timeLog, message); // green

	// TODO: Change console text color
	
	log(messageLog);
}

void AGN::ALoggerLinux::warning(const char *a_info, ...)
{
	// Use the formatting to get the correct log
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog);
	sprintf(messageLog, "\033[1;93m%s| %s\033[0m\n", timeLog, message); // bold & yellow

	// TODO: Change console text color
	
	log(messageLog);
}

void AGN::ALoggerLinux::error(const char *a_info, ...)
{
	// Use the formatting to get the correct log
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog);
	sprintf(messageLog, "\033[1;31m%s| %s\033[0m\n", timeLog, message); // bold & red

	log(messageLog);
}

void AGN::ALoggerLinux::log(const char *a_log)
{
	
	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		printf("%s", a_log);
	}
}

void AGN::ALoggerLinux::getTimeFormatted(char *a_destination)
{
	if (m_timeType == EALogTimeType::RunningTime)
	{
		const float timeElapsed = (float)getTickCount() - m_startTickCount;
		const float totalHours = ((timeElapsed/1000.0f)/60.0f)/60.0f;
		const float minutes = ((totalHours-((int)totalHours))*60.0f);
		const float seconds = ((minutes-((int)minutes))*60.0f);
		const int milliseconds = (int)timeElapsed % 1000;

		std::stringstream milliSecondsString;
		milliSecondsString << std::setfill( '0' ) << std::setw(3) << milliseconds;
		std::stringstream secondsString;
		secondsString << std::setfill( '0' ) << std::setw(2) << (int)seconds;
		std::stringstream minutesString;
		minutesString << std::setfill( '0' ) << std::setw(2) << (int)minutes;
		//std::stringstream hoursString;
		//hoursString << std::setfill( '0' ) << std::setw(2) << (int)totalHours;
	
		std::string time = "" + minutesString.str() + ":" + secondsString.str() + ":" + milliSecondsString.str();
	
		sprintf(a_destination, "%s", time.c_str());
	}
	else if (m_timeType == EALogTimeType::SystemTime)
	{
		// TODO: implement for linux

		/*
		std::time_t rawtime;
		//std::tm* timeinfo;
		char buffer[80];
		struct tm timeinfo;

		std::time(&rawtime);
		//timeinfo = std::localtime_s(&rawtime);
		localtime(&timeinfo, &rawtime);
		std::strftime(buffer, 80, "%H:%M:%S", &timeinfo);

		// TODO: get systemtime in milliseconds too
		//struct timeval tp;
		//gettimeofday(&tp);
		//long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		
		sprintf(a_destination, "%s", buffer);
		*/
	}
}

float AGN::ALoggerLinux::getTickCount()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return unsigned((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}