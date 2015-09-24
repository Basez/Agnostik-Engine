#pragma once 

#include <string>

namespace AGN
{
	class AConversionUtils
	{
	public:
		static std::string getAsHexString(int a_value)
		{
			// TODO: confirm it works
			char buffer[256];
			snprintf(buffer, sizeof(buffer), "0x%X\n", a_value);
			return std::string(buffer);
		};
	};
}