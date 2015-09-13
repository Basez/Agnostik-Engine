#pragma once

#include "ialogger.hpp"

namespace AGN
{
	class ALoggerWin : public IALogger
	{
	public:
		virtual ~ALoggerWin();

		void init(EALogTimeType a_timeType, uint8_t a_outputTypes) override;

		void info(const char *a_info, ...) override;
		void debug(const char *a_debugInfo, ...) override;
		void warning(const char *a_warning, ...) override;
		void error(const char *a_error, ...) override;

	private:
		void createConsole();
		void log(const char *a_log);
		void getTimeFormatted(char *a_destination, unsigned int a_byteSize);

		uint8_t m_outputTypes;
		EALogTimeType m_timeType;
		float m_startTickCount;

	};
}
