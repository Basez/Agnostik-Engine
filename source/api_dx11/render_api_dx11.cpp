#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "render_api_dx11.hpp"
#include "window_dx11.hpp"
#include "device_dx11.hpp"
#include "renderer_dx11.hpp"
#include "config_manager.hpp"
#include "input_dx11.hpp"
#include "application.hpp"
#include "imgui_dx11.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>

AGN::RenderAPIDX11::RenderAPIDX11()
	: m_initialized(false)
	, m_window(nullptr)
	, m_device(nullptr)
	, m_renderer(nullptr)
	, m_imgui(nullptr)
{
}

AGN::RenderAPIDX11::~RenderAPIDX11()
{
	if (m_imgui != nullptr)
	{
		m_imgui->shutdown();
		delete m_imgui;
	}

	delete m_renderer;
	delete m_device;
	delete m_window;
}

bool AGN::RenderAPIDX11::init()
{
	glm::ivec2 windowDimentions;
	windowDimentions.x = g_configManager.getConfigPropertyAsInt32("start_resolution_x");
	windowDimentions.y = g_configManager.getConfigPropertyAsInt32("start_resolution_y");
	m_window = new WindowDX11(windowDimentions);
	
	m_device = new DeviceDX11();
	if (!m_device->init(m_window)) return false;

	m_renderer = new RendererDX11(*this, *m_device, *m_window);
	if (!m_renderer->init()) return false;
	
	m_imgui = new ImGuiDX11();
	m_imgui->init(this);
	
	return true;
}

AGN::IWindow& AGN::RenderAPIDX11::getWindow()
{
	return dynamic_cast<IWindow&>(*m_window);
}

AGN::IDevice& AGN::RenderAPIDX11::getDevice()
{
	return dynamic_cast<IDevice&>(*m_device);
}

AGN::IRenderer& AGN::RenderAPIDX11::getRenderer()
{
	return dynamic_cast<IRenderer&>(*m_renderer);
}

AGN::IImGui& AGN::RenderAPIDX11::getImGui()
{
	return dynamic_cast<AGN::IImGui&>(*m_imgui);
}

void AGN::RenderAPIDX11::enableVSync(bool a_value)
{
	// TODO: Restore
}

void AGN::RenderAPIDX11::handleEvents(bool& a_doQuit)
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
}

void AGN::RenderAPIDX11::logLiveObjects()
{
	m_device->logLiveObjects();
}
