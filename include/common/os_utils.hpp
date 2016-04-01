#pragma once

#include <string>

namespace AGN
{
	namespace OSUtils
	{
		// file utils
		uint32_t getLastlowFileChangeTime(const std::string& a_filename);
		uint32_t getFileSizeBytes(const std::string& a_filename);
		std::string getCanonicalPath(const std::string& a_path);
		std::string getDirectoryOfPath(const std::string& a_path);
		std::string getUpDirectory(const std::string& a_path);
		std::string findFile(const std::string& a_file, const std::string& a_startFolder, const int a_deepLevel, const int a_upLevel);
		std::string getCurrentFolder();
		std::string getExecutableName(const bool a_includeType = false);
		bool fileExists(const std::string& a_file);
		bool directoryExists(const std::string& a_path);
		void createDirectory(const std::string& a_path);
	};
}