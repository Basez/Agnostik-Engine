#pragma once

namespace AGN
{
	enum class EALoggerOutputType
	{
		Window = 1,
		Internal = 2,		// TODO: create internal console (shown inside the main render window)
		OutputDebug = 4,	// windows only, shows logs in visual studio console
	};

	enum class EALogTimeType
	{
		RunningTime,
		SystemTime
	};

	class IALogger
	{
	public:
		virtual ~IALogger(){};

		virtual void init(EALogTimeType a_timeType, uint8_t a_outputTypes) = 0;

		virtual void info(const char *a_info, ...) = 0;
		virtual void debug(const char *a_debugInfo, ...) = 0;
		virtual void warning(const char *a_warning, ...) = 0;
		virtual void error(const char *a_error, ...) = 0;

		static const int MAX_LOG_BUFFER = 1024;
	};
}

// reference to the global logger (implemented in platform specific class)
extern AGN::IALogger& g_log;
