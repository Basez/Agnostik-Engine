
#include "shared.hpp" 
#include "os_utils.hpp"
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

uint32_t AGN::OSUtils::getLastlowFileChangeTime(const std::string& a_filename)
{
	struct stat st;

	if (stat(a_filename.c_str(), &st))
   {
		g_log.error("File (%s) doesnt exist", a_filename.c_str());
		return -1;   	
   }

	//printf("%s: mtime = %lld.%.9ld\n", a_filename.c_str(), (long long)st.st_mtim.tv_sec, st.st_mtim.tv_nsec);
	return (uint32_t)st.st_mtim.tv_sec;
}

/**
	Returns file size of given file in bytes
*/
uint32_t AGN::OSUtils::getFileSizeBytes(const std::string& a_filename)
{
	streampos fsize = 0;
	ifstream file(a_filename, std::ios::binary);

	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.close();

	return (uint32_t)fsize;
}

std::string AGN::OSUtils::getCanonicalPath(const std::string& a_path)
{
	// TODO: expand functionality, currently not a true canonical path
	std::string canonical = a_path;
	std::replace(canonical.begin(), canonical.end(), '\\', '/');
	return canonical;
}

/**
 * Strips the given file path of its name and returns that directory 
 */
std::string AGN::OSUtils::getDirectoryOfPath(const std::string& a_path)
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

std::string AGN::OSUtils::getUpDirectory(const std::string& a_path)
{
	int index = (int)a_path.find_last_of("/"); // find last 2 '\\' (final folder)
	return a_path.substr(0, index).c_str();
}

std::string AGN::OSUtils::findFile(const std::string& a_file, const std::string& a_startFolder, int a_deepLevel, int a_upLevel)
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
					string fileABitDeeper = AGN::OSUtils::findFile(a_file.c_str(), folderFullPath.c_str(), a_deepLevel - 1, 0); // from this point can only go deeper (not up)
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
		string fileAbitUp = AGN::OSUtils::findFile(a_file.c_str(), upFolderPath.c_str(), 0, a_upLevel - 1); // from this point can only go upper (not deeper)

		if (fileAbitUp.size() > 0)
		{
			// FOUND IT IN A HIGHER UP FOLDER
			return fileAbitUp;
		}
	}

	// at this point out of options, we hit our limit up and down, exit recursion
	return "";
}

std::string AGN::OSUtils::getCurrentFolder()
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

std::string AGN::OSUtils::getExecutableName(bool a_includeType)
{
	assert(0);
	return ""; // TODO:
}

bool AGN::OSUtils::fileExists(const std::string& a_file)
{
	ifstream f(a_file.c_str());
	if (f.good())
	{
			f.close();
			return true;
	}
	f.close();
	return false;
}

bool AGN::OSUtils::directoryExists(const std::string& a_path)
{
	struct stat info;

	if (stat(a_path.c_str(), &info) != 0)
	{
			return false;
	}
	else if (info.st_mode & S_IFDIR)
	{
			return true;
	}
	return false;
}


void AGN::OSUtils::createDirectory(const std::string& a_path)
{
	std::vector<std::string> directoriesToCreate;
	std::string currentPath = a_path;
	
	// get all paths
	while(1)
	{
		if (OSUtils::directoryExists(currentPath) || currentPath.length() == 0) break;

		directoriesToCreate.push_back(currentPath);

		currentPath = AGN::OSUtils::getUpDirectory(currentPath);

		assert(directoriesToCreate.size() < 10); // safety check
	}

	for (int32_t i = (int32_t)directoriesToCreate.size() - 1; i >= 0; i--)
	{
		//int mkdir(const char *path, mode_t mode);
		int result = mkdir(directoriesToCreate[i].c_str(), 0);
		assert(result == 0);
	}
}
