#include "asharedh.hpp"
#include "aaplication.hpp"
#include "arender_api_gl.hpp"

#include <Windows.h>

using namespace AGN;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_log.init(LogTimeType::RunningTime, LoggerOutputType::Window | LoggerOutputType::OutputDebug);

	IARenderAPI* renderAPI = new ARenderAPIGL();

	g_application.run(renderAPI);

	g_application.cleanup(); 

	delete renderAPI;

	return 0;
}
