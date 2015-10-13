#include "asharedh.hpp"
#include "aaplication.hpp"
#include "arender_api_gl.hpp"
#include "aconfigmanager.hpp"
#include "aosutils.hpp"

using namespace AGN;

int main(int /*argc*/, char* /*argv*/[])
{
	g_log.init(EALogTimeType::RunningTime, (int)EALoggerOutputType::Window);

	int* testIntPointer = nullptr;
	g_log.debug("pointer size: %i", sizeof(testIntPointer));
	
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
