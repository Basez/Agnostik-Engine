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
	// TODO:
	//m_device = new ADeviceDX11();
	//m_renderer = new ADeviceDX11();
}

bool AGN::ARenderAPIDX11::init()
{
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
