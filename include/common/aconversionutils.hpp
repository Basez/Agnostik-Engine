#pragma once 

#include <map>
#include <string>
#include <sstream>

namespace AGN
{
	class AConversionUtils
	{
	public:
		static std::string getAsHexString(int a_value)
		{
			std::stringstream stream;
			stream << std::string("0x") << std::hex << a_value;
			return std::string(stream.str());
		};
	};
}