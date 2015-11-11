#include "shared.hpp"
#include <fstream>

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "config_manager.hpp"
#include "os_utils.hpp"

using namespace std;

AGN::ConfigManager configTemp = AGN::ConfigManager();
AGN::ConfigManager& g_configManager = configTemp;

bool AGN::ConfigManager::parseConfigFile(std::string a_configFile)
{
	g_log.info("parsing config file..");

	string configFileFolder = AGN::OSUtils::getDirectoryOfPath(a_configFile); // (aka root folder)

	std::ifstream file;
	file.open(a_configFile, std::ifstream::in);
	if (!file.is_open())
	{
		g_log.error("Could not open config file!");
		return false;
	}

	std::string line;

	while (std::getline(file, line))
	{
		// parse shader & destination folders
		if (line.substr(0, 1) == "#" || line.length() == 0) continue; // ignore comments or empty lines

		string key, value;

		AGN::ConfigManager::getPropertiesOfLine(line, key, value);

		// check if value needs certain processing (eg: if its a relative path)
		if (value.substr(0, 1).compare("/") == 0)
		{
			// value is a relative path, lets make it a full path
			value = configFileFolder + value;
		}

		m_configProperties[key] = value;

		g_log.info("%s: %s", key.c_str(), value.c_str());
	}
	
	return true;
}

std::string AGN::ConfigManager::getConfigProperty(std::string a_key)
{
	if (m_configProperties.find(a_key) == m_configProperties.end())
	{
		g_log.error("Config property '%s' was not found!", a_key.c_str());
		return "error";
	}

	return m_configProperties[a_key];
}

// helper function that separates key from value on the line for example:
// LobbyIP=[127.0.0.1] --> Key: "LobbyIP" --> Value: "127.0.0.1"
void AGN::ConfigManager::getPropertiesOfLine(string a_line, string &out_key, string &out_value)
{
	size_t foundIndex = (size_t)a_line.find_first_of("=[");

	if (foundIndex == string::npos)
	{
		printf("cant find '=' in property line !\n");
		return;
	}

	size_t beginOfProperty = foundIndex + 2;
	size_t propertyLength = (size_t)a_line.find_last_of("]") - beginOfProperty;

	out_key = a_line.substr(0, foundIndex);
	out_value = a_line.substr(beginOfProperty, propertyLength);
}

int32_t AGN::ConfigManager::getConfigPropertyAsInt32(std::string a_key)
{
	std::string propertyAsString = getConfigProperty(a_key);

	return atoi(propertyAsString.c_str());
}

bool AGN::ConfigManager::getConfigPropertyAsBool(std::string a_key)
{
	std::string propertyAsString = getConfigProperty(a_key);

	return propertyAsString.compare("true") == 0;
}
