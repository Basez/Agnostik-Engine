#include "shared.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "config_manager.hpp"
#include "os_utils.hpp"
#include "render_api_dx11.hpp"

using namespace AGN;

int main (int argc, char* argv[])
{
	// TODO: detect mode 
	
	
	// load configurations
	std::string currentFolder = OSUtils::getCurrentFolder();
	std::string configFile = OSUtils::findFile("config.ini", currentFolder.c_str(), 3, 3);
	std::string rootFolder = OSUtils::getDirectoryOfPath(configFile);
	g_configManager.parseConfigFile(configFile);

	// show log output
	if (g_configManager.getConfigPropertyAsBool("enable_log_window"))
	{
		g_log.init(ELogTimeType::RunningTime, (uint8_t)ELoggerOutputType::Window | (uint8_t)ELoggerOutputType::OutputDebug);
	}
	
	// parse stuff

	return 0;
}
