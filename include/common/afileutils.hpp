#pragma once

#include <string>

namespace AGN
{
	class AFileUtils
	{
	public:
		static int getLastlowFileChangeTime(char* a_filename);
		static int getFileSizeBytes(std::string a_filename);
		static std::string getDirectoryOfPath(std::string a_path);
		static std::string getUpDirectory(std::string a_path);
		static std::string getPathRelativeToPath(std::string a_originPath, std::string a_relativeToOrigin);
		static std::string findFile(const char* a_file, const char* a_startFolder, int a_deepLevel, int a_upLevel);
		static std::string getCurrentFolder();
		static std::string getExecutableName(bool a_includeType = false);
	};
}