#include "shared.hpp"
#include "application.hpp"
#include "render_api_gl.hpp"
#include "config_manager.hpp"
#include "os_utils.hpp"

using namespace AGN;

int main(int /*argc*/, char* /*argv*/[])
{
	// load configurations
	std::string currentFolder = OSUtils::getCurrentFolder();
	std::string configFile = OSUtils::findFile("config.ini", currentFolder.c_str(), 3, 3);
	std::string rootFolder = OSUtils::getDirectoryOfPath(configFile);
	g_configManager.parseConfigFile(configFile);

	// show log output
	if (g_configManager.getConfigPropertyAsBool("enable_log_window"))
	{
		g_log.init(ELogTimeType::RunningTime, (int)ELoggerOutputType::Window);
	}
	
	IRenderAPI* renderAPI = new RenderAPIGL();

	g_application.run(renderAPI);

	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}
