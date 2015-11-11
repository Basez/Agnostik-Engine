#pragma once

namespace AGN
{
	enum class ELoggerOutputType
	{
		Window = 1,
		Internal = 2,		// TODO: create internal console (shown inside the main render window)
		OutputDebug = 4,	// windows only, shows logs in visual studio console
	};

	enum class ELogTimeType
	{
		RunningTime,
		SystemTime
	};

	class ILogger
	{
	public:
		virtual ~ILogger(){};

		virtual void init(ELogTimeType a_timeType, uint8_t a_outputTypes) = 0;
		virtual void cleanup() = 0;

		virtual void info(const char *a_info, ...) = 0;
		virtual void debug(const char *a_debugInfo, ...) = 0;
		virtual void warning(const char *a_warning, ...) = 0;
		virtual void error(const char *a_error, ...) = 0;

		static const int MAX_LOG_BUFFER = 1024;
	};
}

// reference to the global logger (implemented in platform specific class)
extern AGN::ILogger& g_log;
