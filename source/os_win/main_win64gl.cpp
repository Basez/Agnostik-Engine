#include "asharedh.hpp"
#include "aaplication.hpp"
#include "arender_api_gl.hpp"
#include "aconfigmanager.hpp"
#include "afileutils.hpp"

#include <Windows.h>

using namespace AGN;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_log.init(LogTimeType::RunningTime, LoggerOutputType::Window | LoggerOutputType::OutputDebug);

	int* testIntPointer = nullptr;

	g_log.debug("pointer size: %i", sizeof(testIntPointer));
	g_log.info("info test");
	g_log.debug("debug test");
	g_log.warning("warning test");
	g_log.error("error test");
	
	// load configurations
	std::string currentFolder = AFileUtils::getCurrentFolder();
	std::string configFile = AFileUtils::findFile("config.ini", currentFolder.c_str(), 3, 3);
	std::string rootFolder = AFileUtils::getDirectoryOfPath(configFile);
	g_configManager.parseConfigFile(configFile);


	IARenderAPI* renderAPI = new ARenderAPIGL();

	g_application.run(renderAPI);
	
	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}
