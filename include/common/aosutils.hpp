#pragma once

#include <string>

namespace AGN
{
	namespace AOSUtils
	{
		// file utils
		int getLastlowFileChangeTime(std::string a_filename);
		int getFileSizeBytes(std::string a_filename);
		std::string getDirectoryOfPath(std::string a_path);
		std::string getUpDirectory(std::string a_path);
		std::string getPathRelativeToPath(std::string a_originPath, std::string a_relativeToOrigin);
		std::string findFile(std::string a_file, std::string a_startFolder, int a_deepLevel, int a_upLevel);
		std::string getCurrentFolder();
		std::string getExecutableName(bool a_includeType = false);

		size_t cStringCopy(char *a_destination, char const *a_source, size_t a_sizeBytes);
		size_t cStringConcatenate(char *a_destination, char const *a_source, size_t a_sizeBytes);
	};
}