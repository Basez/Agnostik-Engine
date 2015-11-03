#pragma once

#include <string>

namespace AGN
{
	namespace OSUtils
	{
		// file utils
		int getLastlowFileChangeTime(std::string a_filename);
		int getFileSizeBytes(std::string a_filename);
		std::string getDirectoryOfPath(std::string a_path);
		std::string getUpDirectory(std::string a_path);
		std::string findFile(std::string a_file, std::string a_startFolder, int a_deepLevel, int a_upLevel);
		std::string getCurrentFolder();
		std::string getExecutableName(bool a_includeType = false);
	};
}