#include "asharedh.hpp"
#include "alogger_win.hpp"

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sstream>
#include <ctime>
#include <time.h>
#include <iomanip>

HANDLE g_hConsole = nullptr;
FILE* g_pCout = nullptr;
FILE* g_pCin = nullptr;

AGN::ALoggerWin LoggerWin = AGN::ALoggerWin();
AGN::IALogger& g_log = LoggerWin;

AGN::ALoggerWin::~ALoggerWin()
{
	if (g_hConsole != nullptr) CloseHandle(g_hConsole);
	if (g_pCout != nullptr) fclose(g_pCout);
	if (g_pCin != nullptr) fclose(g_pCin);
}

void AGN::ALoggerWin::init(EALogTimeType a_timeType, uint8_t a_outputTypes)
{
	m_outputTypes = a_outputTypes;
	m_timeType = a_timeType;

	m_startTickCount = (float)GetTickCount();

	if (m_outputTypes & (int)EALoggerOutputType::Window) createConsole();
}

void AGN::ALoggerWin::createConsole()
{
	// allocate a console for this app
	AllocConsole();

	freopen_s(&g_pCin, "conin$", "r", stdin);
	freopen_s(&g_pCout, "conout$", "w", stdout);
	freopen_s(&g_pCout, "conout$", "w", stderr);

	g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void AGN::ALoggerWin::info(const char *a_info, ...)
{
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// Change console text color
	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN);
	}

	log(messageLog);
}

void AGN::ALoggerWin::debug(const char *a_info, ...)
{
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// Change console text color
	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	}

	log(messageLog);
}

void AGN::ALoggerWin::warning(const char *a_info, ...)
{
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];

	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// Change console text color
	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN);
	}

	log(messageLog);

}

void AGN::ALoggerWin::error(const char *a_info, ...)
{
	// Use the formatting to get the correct log and copy it to the messageLog
	va_list arguments;
	char message[IALogger::MAX_LOG_BUFFER];
	
	va_start(arguments, a_info);
	vsprintf_s(message, a_info, arguments);
	va_end(arguments);

	char messageLog[IALogger::MAX_LOG_BUFFER];
	char timeLog[256];
	getTimeFormatted(timeLog, 256);
	sprintf_s(messageLog, sizeof(messageLog), "%s| %s \n", timeLog, message);

	// Change console text color
	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED);
	}

	log(messageLog);

}

void AGN::ALoggerWin::log(const char *a_log)
{

	if (m_outputTypes & (int)EALoggerOutputType::Window)
	{
		printf("%s", a_log);
	}

	if (m_outputTypes & (int)EALoggerOutputType::OutputDebug)
	{
		OutputDebugString((LPCSTR)a_log);
	}
}

void AGN::ALoggerWin::getTimeFormatted(char *a_destination, unsigned int a_byteSize)
{
	if (m_timeType == EALogTimeType::RunningTime)
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
	else if (m_timeType == EALogTimeType::SystemTime)
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
}