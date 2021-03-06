#pragma once

#include "i_logger.hpp"

namespace AGN
{
	class LoggerLinux : public ILogger
	{
	public:
		~LoggerLinux();
		void init(ELogTimeType a_timeType, uint8_t a_outputTypes) override;
		void cleanup() override;

		void info(const char *a_info, ...) override;
		void debug(const char *a_debugInfo, ...) override;
		void warning(const char *a_warning, ...) override;
		void error(const char *a_error, ...) override;

	private:
		void log(const char *a_log);
		void getTimeFormatted(char *a_destination);
		float getTickCount();

		uint8_t m_outputTypes;
		ELogTimeType m_timeType;
		float m_startTickCount;

	};
}
