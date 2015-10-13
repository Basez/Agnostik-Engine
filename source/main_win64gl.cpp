#include "asharedh.hpp"
#include "aaplication.hpp"
#include "arender_api_gl.hpp"
#include "aconfigmanager.hpp"
#include "aosutils.hpp"

// TODO: lean and mean?
#include <Windows.h>

using namespace AGN;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_log.init(EALogTimeType::RunningTime, (uint8_t)EALoggerOutputType::Window | (uint8_t)EALoggerOutputType::OutputDebug);

	g_log.debug("pointer size check: %i", sizeof(int*));

	// load configurations
	std::string currentFolder = AOSUtils::getCurrentFolder();
	std::string configFile = AOSUtils::findFile("config.ini", currentFolder.c_str(), 3, 3);
	std::string rootFolder = AOSUtils::getDirectoryOfPath(configFile);
	g_configManager.parseConfigFile(configFile);

	IARenderAPI* renderAPI = new ARenderAPIGL();

	g_application.run(renderAPI);
	
	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}
