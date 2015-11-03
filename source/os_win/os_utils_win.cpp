#include "shared.hpp" 
#include "os_utils.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dirent/dirent.h>
#include <iostream>
#include <fstream>

using namespace std;

int AGN::OSUtils::getLastlowFileChangeTime(std::string a_filename)
{
	WIN32_FIND_DATA findData;
	HANDLE hHandle = FindFirstFile((LPCSTR)a_filename.c_str(), &findData);
	int success = GetFileTime(hHandle, NULL, NULL, NULL);

	return findData.ftLastWriteTime.dwLowDateTime;
}

/**
	Returns file size of given file in bytes
*/
int AGN::OSUtils::getFileSizeBytes(std::string a_filename)
{
	streampos fsize = 0;
	ifstream file(a_filename, std::ios::binary);

	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.close();

	return (int)fsize;
}

/**
	Strips the given file path of its name and returns that directory
*/
std::string AGN::OSUtils::getDirectoryOfPath(std::string a_path)
{
	std::string pathAsString = a_path;

	// check if its a / or \\ path!
	int rsIndex = (int)pathAsString.find_last_of('/');
	if (rsIndex != -1) return pathAsString.substr(0, rsIndex).c_str();

	int lsIndex = (int)pathAsString.find_last_of("\\\\");
	if (lsIndex != -1) return pathAsString.substr(0, lsIndex).c_str();

	g_log.error("cant get directory of path: %s --> weird format", a_path.c_str());

	return "";
}

/**
	Returns the path 1 directory up
*/
std::string AGN::OSUtils::getUpDirectory(std::string a_path)
{
	int index = (int)a_path.find_last_of("\\\\"); // find last 2 '\\' (final folder)
	return a_path.substr(0, index).c_str();
}

std::string AGN::OSUtils::findFile(std::string a_file, std::string a_startFolder, int a_deepLevel, int a_upLevel)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(a_startFolder.c_str())) != NULL)
	{
		// print all the files and directories within directory
		while ((ent = readdir(dir)) != NULL)
		{
			// skip windows .. folders (what are these??)
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{
				//Log.debug("Found weird %s folder.. skipping", ent->d_name);
			}
			else if (ent->d_type == DT_DIR)
			{
				//Log.debug("Found folder: %s", ent->d_name);

				if (a_deepLevel > 0)
				{
					// append string
					string folderFullPath = string(string(a_startFolder.c_str()) + string("\\") + string(ent->d_name)).c_str();
					string fileABitDeeper = AGN::OSUtils::findFile(a_file.c_str(), folderFullPath.c_str(), a_deepLevel - 1, 0); // from this point can only go deeper (not up)
					if (fileABitDeeper.size() > 0)
					{
						// Found file in a deeper folder
						return fileABitDeeper;
					}
				}
			}
			else if (strcmp(ent->d_name, a_file.c_str()) == 0)
			{
				//Log.debug("Found the file");
				std::string foundFile = ent->d_name;
				closedir(dir);

				string fileLocation = string(a_startFolder.c_str()) + string("\\") + foundFile;
				return fileLocation;
			}
			else
			{
				//Log.debug("not the file: %s", ent->d_name);
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
		string fileAbitUp = AGN::OSUtils::findFile(a_file.c_str(), upFolderPath.c_str(), 0, a_upLevel - 1); // from this point can only go upper (not deeper)

		if (fileAbitUp.size() > 0)
		{
			// found the file in a higher up folder
			return fileAbitUp;
		}
	}

	// at this point out of options, we hit our limit up and down, exit recursion
	return "";
}

std::string AGN::OSUtils::getCurrentFolder()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos).c_str();
}

std::string AGN::OSUtils::getExecutableName(bool a_includeType)
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/") + 1;

	string nameWithType = string(buffer).substr(pos, strlen(buffer));

	if (a_includeType) return nameWithType.c_str();

	string nameWithoutType = nameWithType.substr(0, nameWithType.find_last_of(".exe") - 3);

	return nameWithoutType.c_str();
}