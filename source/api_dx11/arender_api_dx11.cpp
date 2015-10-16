#include "asharedh.hpp"
#include "arender_api_dx11.hpp"
#include "awindow_dx11.hpp"
#include "adevice_dx11.hpp"
#include "arenderer_dx11.hpp"
#include "aconfigmanager.hpp"
#include "ainput_dx11.hpp"
#include "aaplication.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

AGN::ARenderAPIDX11::ARenderAPIDX11()
	: m_initialized(false)
	, m_window(nullptr)
	, m_device(nullptr)
	, m_renderer(nullptr)
{

}

bool AGN::ARenderAPIDX11::init()
{
	glm::ivec2 windowDimentions;
	windowDimentions.x = g_configManager.getConfigPropertyAsInt32("start_resolution_x");
	windowDimentions.y = g_configManager.getConfigPropertyAsInt32("start_resolution_y");
	m_window = new AWindowDX11(windowDimentions);

	m_device = new ADeviceDX11(m_window);
	if (!m_device->init()) return false;

	m_renderer = new ARendererDX11(*this, *m_device, *m_window);
	if (!m_renderer->init()) return false;

	return true;
}

AGN::IAWindow& AGN::ARenderAPIDX11::getWindow()
{
	return dynamic_cast<IAWindow&>(*m_window);
}

AGN::IADevice& AGN::ARenderAPIDX11::getDevice()
{
	return dynamic_cast<IADevice&>(*m_device);
}

AGN::IARenderer& AGN::ARenderAPIDX11::getRenderer()
{
	return dynamic_cast<IARenderer&>(*m_renderer);
}

void AGN::ARenderAPIDX11::enableVSync(bool a_value)
{
	// TODO: Restore
}

void AGN::ARenderAPIDX11::handleEvents(bool& a_doQuit)
{
	a_doQuit = false;

	MSG msg = { 0 };

	//static DWORD previousTime = timeGetTime();
	//static const float targetFramerate = 30.0f;
	//static const float maxTimeStep = 1.0f / targetFramerate;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	// translate virtual-key messages into character messages
		DispatchMessage(&msg); // dispatch the message to the appropriate window’s procedure function

		if (msg.message == WM_QUIT)
		{
			a_doQuit = true;
		}
	}

	

	// TODO: Restore
}
