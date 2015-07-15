#include "asharedh.hpp"
#include "alogger_linux.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <sys/io.h>
#include <sstream>
#include <ctime>
#include <time.h>
#include <iomanip>

AGN::ALoggerLinux loggerLinux = AGN::ALoggerLinux();
AGN::IALogger& g_log = loggerLinux;

void AGN::ALoggerLinux::init(LogTimeType a_timeType, uint8_t a_outputTypes)
{
	/*
	m_outputTypes = a_outputTypes;
	m_timeType = a_timeType;

	m_startTickCount = (float)GetTickCount();
	*/
}

void AGN::ALoggerLinux::info(const char *a_info, ...)
{
	/*
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[512];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[512];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// TODO: Change console text color
	
	log(messageLog);
	*/
}

void AGN::ALoggerLinux::debug(const char *a_info, ...)
{
	/*
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[512];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[512];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// TODO: Change console text color

	log(messageLog);
	*/
}

void AGN::ALoggerLinux::warning(const char *a_info, ...)
{
	/*
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[512];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[512];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// TODO: Change console text color

	log(messageLog);
	*/

}

void AGN::ALoggerLinux::error(const char *a_info, ...)
{
	/*
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[512];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[512];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// TODO: Change console text color

	log(messageLog);
	*/
}

void AGN::ALoggerLinux::log(const char *a_log)
{
	/*
	if (m_outputTypes & LoggerOutputType::Window)
	{
		printf("%s", a_log);
	}
	*/
}

void AGN::ALoggerLinux::getTimeFormatted(char *a_destination, unsigned int a_byteSize)
{
	/*
	if (m_timeType == LogTimeType::RunningTime)
	{
		const float timeElapsed = (float)GetTickCount() - m_startTickCount;
		const float totalHours = ((timeElapsed / 1000.0f) / 60.0f) / 60.0f;
		const float minutes = ((totalHours - ((int)totalHours))*60.0f);
		const float seconds = ((minutes - ((int)minutes))*60.0f);
		const int milliseconds = (int)timeElapsed % 1000;

		std::stringstream milliSecondsString;
		milliSecondsString << std::setfill('0') << std::setw(3) << milliseconds;
		std::stringstream secondsString;
		secondsString << std::setfill('0') << std::setw(2) << (int)seconds;
		std::stringstream minutesString;
		minutesString << std::setfill('0') << std::setw(2) << (int)minutes;
		//std::stringstream hoursString;
		//hoursString << std::setfill( '0' ) << std::setw(2) << (int)totalHours;

		std::string time = "" + minutesString.str() + ":" + secondsString.str() + ":" + milliSecondsString.str();

		sprintf_s(a_destination, a_byteSize, "%s", time.c_str());
	}
	else if (m_timeType == LogTimeType::SystemTime)
	{
		std::time_t rawtime;
		//std::tm* timeinfo;
		char buffer[80];
		struct tm timeinfo;

		std::time(&rawtime);
		//timeinfo = std::localtime_s(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		std::strftime(buffer, 80, "%H:%M:%S", &timeinfo);

		// TODO: get systemtime in milliseconds too
		//struct timeval tp;
		//gettimeofday(&tp);
		//long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

		sprintf_s(a_destination, a_byteSize, "%s", buffer);
	}
	*/
}