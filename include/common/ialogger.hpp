#pragma once

namespace AGN
{
	enum LoggerOutputType
	{
		Window = 1,
		Internal = 2, // TODO: create internal console (shown inside the main render window)
		OutputDebug = 4, // windows only
	};

	enum class LogTimeType
	{
		RunningTime,
		SystemTime
	};

	class IALogger
	{
	public:
		virtual void init(LogTimeType a_timeType, uint8_t a_outputTypes) = 0;

		virtual void info(const char *a_info, ...) = 0;
		virtual void debug(const char *a_debugInfo, ...) = 0;
		virtual void warning(const char *a_warning, ...) = 0;
		virtual void error(const char *a_error, ...) = 0;
	};
}

// reference to the global logger (implemented in platform specific class)
extern AGN::IALogger& g_log;
