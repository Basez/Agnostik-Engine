#include "asharedh.hpp" 
#include "aosutils.hpp"
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <algorithm>

using namespace std;

int AGN::AOSUtils::getLastlowFileChangeTime(std::string a_filename)
{
	return -1; // TODO:
}

int AGN::AOSUtils::getFileSizeBytes(std::string a_filename)
{
	return -1; // TODO:
}

/**
 * Strips the given file path of its name and returns that directory 
 */
std::string AGN::AOSUtils::getDirectoryOfPath(std::string a_path)
{
	string pathAsString = a_path;

	// check if its a / or \\ path!
	int rsIndex = (int)pathAsString.find_last_of('/');
	if (rsIndex != -1) return pathAsString.substr(0, rsIndex).c_str();

	int lsIndex = (int)pathAsString.find_last_of("\\\\");
	if (lsIndex != -1) return pathAsString.substr(0, lsIndex).c_str();

	g_log.error("cant get directory of path: %s --> weird format", a_path.c_str());

	return "";
}

std::string AGN::AOSUtils::getUpDirectory(std::string a_path)
{
	int index = (int)a_path.find_last_of("/"); // find last 2 '\\' (final folder)
	return a_path.substr(0, index).c_str();
}

std::string AGN::AOSUtils::getPathRelativeToPath(std::string a_originPath, std::string a_relativeToOrigin)
{
	string relativePath = "";
	relativePath += getDirectoryOfPath(a_originPath);	// start with directory of origin
	relativePath += "/";
	relativePath += a_relativeToOrigin;

	return relativePath;
}

std::string AGN::AOSUtils::findFile(std::string a_file, std::string a_startFolder, int a_deepLevel, int a_upLevel)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(a_startFolder.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL)
		{
			// skip windows .. folders (what are these??)
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				//g_log.debug("Found weird %s folder.. skipping", ent->d_name);
			}
			else if (ent->d_type == DT_DIR)
			{
				//g_log.debug("Found folder: %s", ent->d_name);

				if (a_deepLevel > 0)
				{
					// append string
					string folderFullPath = string(string(a_startFolder.c_str()) + string("\\") + string(ent->d_name)).c_str();
					string fileABitDeeper = AGN::AOSUtils::findFile(a_file.c_str(), folderFullPath.c_str(), a_deepLevel - 1, 0); // from this point can only go deeper (not up)
					if (fileABitDeeper.size() > 0)
					{
						// FOUND IT IN A DEEPER FOLDER
						return fileABitDeeper;
					}
				}
			}
			else if (strcmp(ent->d_name, a_file.c_str()) == 0)
			{
				//g_log.debug("FOUND THE FILE");
				std::string foundFile = ent->d_name;
				closedir(dir);

				string fileLocation = string(a_startFolder.c_str()) + string("/") + foundFile;
				return fileLocation;
			}
			else
			{
				//g_log.debug("nope: %s", ent->d_name);
			}
		}
		closedir(dir);
	}
	else
	{
		g_log.error("error opening folder: %s", a_startFolder.c_str());
		return "";
	}

	// not yet found, lets go up (if we can)
	if (a_upLevel > 0)
	{
		string upFolderPath = getUpDirectory(a_startFolder.c_str());
		string fileAbitUp = AGN::AOSUtils::findFile(a_file.c_str(), upFolderPath.c_str(), 0, a_upLevel - 1); // from this point can only go upper (not deeper)

		if (fileAbitUp.size() > 0)
		{
			// FOUND IT IN A HIGHER UP FOLDER
			return fileAbitUp;
		}
	}

	// at this point out of options, we hit our limit up and down, exit recursion
	return "";
}

std::string AGN::AOSUtils::getCurrentFolder()
{
 	char buffer[PATH_MAX] = {0};
	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int32_t pathLength = readlink(szTmp, buffer, PATH_MAX);
	int32_t bytes = min(pathLength, PATH_MAX - 1);
	if(bytes >= 0) buffer[bytes] = '\0';

	string::size_type pos = string(buffer).find_last_of("\\/");
	string fullPath = string(buffer).substr(0, pos).c_str();

	return fullPath;
}

std::string AGN::AOSUtils::getExecutableName(bool a_includeType)
{
	return ""; // TODO:
}

size_t AGN::AOSUtils::cStringCopy(char *a_destination, char const *a_source, size_t a_sizeBytes)
{
	return snprintf(a_destination, a_sizeBytes, "%s", a_source);
}

size_t AGN::AOSUtils::cStringConcatenate(char *a_destination, char const *a_source, size_t a_sizeBytes)
{
	return snprintf(a_destination, a_sizeBytes, "%s%s", a_destination, a_source);
}

