#pragma once 

#include <map>

namespace AGN
{
	class AConfigManager
	{
	public:
		bool parseConfigFile(std::string a_configFilePath);
		std::string getConfigProperty(std::string a_key);

		//const char* getFullPathFromRoot(std::string a_relativePath);
		//const char* s_rootFolder;

	private:
		std::map<std::string, std::string> m_configProperties;
		static void getPropertiesOfLine(std::string a_line, std::string &out_key, std::string &out_value);

	};
}

extern AGN::AConfigManager& g_configManager;