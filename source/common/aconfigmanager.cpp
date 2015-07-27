#include "asharedh.hpp"
#include "aconfigmanager.hpp"

#include <iostream>
#include <fstream>

using namespace std;

AGN::AConfigManager configTemp = AGN::AConfigManager();
AGN::AConfigManager& g_configManager = configTemp;

//std::string AConfigManager::s_rootFolder = "";

/*
std::string AConfigManager::getFullPathFromRoot(std::string a_relativePath)
{
	return s_rootFolder + string("\\") + a_relativePath;
}
*/

bool AGN::AConfigManager::parseConfigFile(std::string a_configFile)
{
	/*
	g_log.info("parsing config file..");

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

		AGN::AConfigManager::getPropertiesOfLine(line, key, value);

		m_configProperties[key] = value;

		g_log.info("property %s : %s", key.c_str(), value.c_str());
	}
	*/
	return true;
}

std::string AGN::AConfigManager::getConfigProperty(std::string a_key)
{
	if (m_configProperties.find(a_key) == m_configProperties.end())
	{
		g_log.error("Config property with key: %s was not found!", a_key);
		return "error";
	}

	return m_configProperties[a_key];
}

// helper function that seperates key from value on the line for example:
// LobbyIP=[127.0.0.1] --> Key: "LobbyIP" --> Value: "127.0.0.1"
void AGN::AConfigManager::getPropertiesOfLine(string a_line, string &out_key, string &out_value)
{
	/*
	int foundIndex = a_line.find_first_of("=[");

	if (foundIndex == string::npos)
	{
		printf("cant find '=' in property line !\n");
		return;
	}

	int beginOfProperty = foundIndex + 2;
	int propertyLength = a_line.find_last_of("]") - beginOfProperty;

	out_key = a_line.substr(0, foundIndex);
	out_value = a_line.substr(beginOfProperty, propertyLength);
	*/
}

