#include "asharedh.hpp"
#include "arender_api_dx11.hpp"
#include "awindow_dx11.hpp"
#include "adevice_dx11.hpp"
#include "arenderer_dx11.hpp"
#include "aconfigmanager.hpp"
#include "ainput_dx11.hpp"
#include "aaplication.hpp"

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
	m_device->init();

	m_renderer = new ARendererDX11(m_device, m_window);
	m_renderer->init();

	return false; // TODO:
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

void AGN::ARenderAPIDX11::handleEvents()
{
	// TODO: Restore
}
