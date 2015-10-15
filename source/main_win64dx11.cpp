#include "asharedh.hpp"
#include "aaplication.hpp"
#include "aconfigmanager.hpp"
#include "aosutils.hpp"
#include "arender_api_dx11.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace AGN;

int WINAPI WinMain(HINSTANCE a_hInstance, HINSTANCE a_hPrevInstance, LPSTR a_lpCmdLine, int a_nShowCmd)
{
	UNREFERENCED_PARAMETER(a_hInstance);
	UNREFERENCED_PARAMETER(a_hPrevInstance);
	UNREFERENCED_PARAMETER(a_lpCmdLine);
	UNREFERENCED_PARAMETER(a_nShowCmd);

	g_log.init(EALogTimeType::RunningTime, (uint8_t)EALoggerOutputType::Window | (uint8_t)EALoggerOutputType::OutputDebug);

	g_log.debug("pointer size check: %i", sizeof(int*));

	// load configurations
	std::string currentFolder = AOSUtils::getCurrentFolder();
	std::string configFile = AOSUtils::findFile("config.ini", currentFolder.c_str(), 3, 3);
	std::string rootFolder = AOSUtils::getDirectoryOfPath(configFile);
	g_configManager.parseConfigFile(configFile);

	IARenderAPI* renderAPI = new ARenderAPIDX11();

	g_application.run(renderAPI);
	
	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}
