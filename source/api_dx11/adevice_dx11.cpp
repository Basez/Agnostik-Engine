#include "asharedh.hpp"
#include "adevice_dx11.hpp"
#include "amesh_dx11.hpp"
#include "atexture_dx11.hpp"
#include "ashader_dx11.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "awindow_dx11.hpp"
#include "aconfigmanager.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace glm;

AGN::ADeviceDX11::ADeviceDX11(class AWindowDX11* a_window)
	: m_window(a_window)
{

}

bool AGN::ADeviceDX11::init()
{
	assert(m_window->getWindowHandle());

	RECT clientRect;
	GetClientRect(m_window->getWindowHandle(), &clientRect);

	// Compute the exact client dimensions. This will be used
	// to initialize the render targets for our swap chain.
	unsigned int clientWidth = clientRect.right - clientRect.left;
	unsigned int clientHeight = clientRect.bottom - clientRect.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	bool vsync = g_configManager.getConfigPropertyAsBool("vsync");

	swapChainDesc.BufferCount = 1;												// the number of buffers in the swap chain
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// pixel format of the display in this case: a 4-component 32-bit unsigned normalized integer format that supports 8 bits per channel including alpha
	swapChainDesc.BufferDesc.RefreshRate = queryRefreshRate(vsync);				// refresh rate in hertz, 0/1 to specify an unbounded refresh rate.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;				// describes the surface usage and CPU access options for the back buffer in this case: Use the surface or resource as an output render target.
	swapChainDesc.OutputWindow = m_window->getWindowHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;						// describes options for handling the contents of the presentation buffer after presenting a surface
	swapChainDesc.Windowed = TRUE;												// output is in windowed mode.

	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// These are the feature levels that we will accept.
	//  The device will be created with the highest feature level (first in array) that is supported by the end-user’s hardware.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// This will be the feature level that 
	// is used to create our device and swap chain.
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,					// video adapter to use when creating a device // nullptr == default
		D3D_DRIVER_TYPE_HARDWARE,	// The D3D driver which implements the device.
		nullptr,					// handle to a DLL that implements a software rasterizer
		createDeviceFlags,
		featureLevels,				// determine the order of feature levels to attempt to create.
		_countof(featureLevels),
		D3D11_SDK_VERSION,			// SDK version
		&swapChainDesc,
		&m_d3d11SwapChain,			// swap chain description which was created earlier.
		&m_d3d11Device,				// ID3D11Device object that represents the device created.
		&featureLevel,				// the first element in an array of feature levels supported by the device.
		&m_d3d11DeviceContext);

	// if failed, initialize DX_11_0 instead of DX_11_1
	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_d3d11SwapChain, &m_d3d11Device, &featureLevel,
			&m_d3d11DeviceContext);

		// failed again?
		if (FAILED(hr))
		{
			g_log.error("Failed to create D3D11 Device And Swap Chain");
			return false;
		}
	}

	return true;
}

// reference: http://www.rastertek.com/dx11tut03.html
DXGI_RATIONAL AGN::ADeviceDX11::queryRefreshRate(bool a_vsync)
{
	DXGI_RATIONAL refreshRate = { 0, 1 };
	if (a_vsync)
	{
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		DXGI_MODE_DESC* displayModeList;

		// Create a DirectX graphics interface factory.
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(hr))
		{
			m_window->showMessageBox("Query Refresh Rate", "Could not create DXGIFactory instance");
			return refreshRate;
		}

		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr))
		{
			m_window->showMessageBox("Query Refresh Rate", "Failed to enumerate adapters.");
			return refreshRate;
		}

		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr))
		{
			m_window->showMessageBox("Query Refresh Rate", "Failed to enumerate adapter outputs");
			return refreshRate;
		}

		UINT numDisplayModes;
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
		if (FAILED(hr))
		{
			m_window->showMessageBox("Query Refresh Rate", "Failed to query display mode list.");
			return refreshRate;
		}

		displayModeList = new DXGI_MODE_DESC[numDisplayModes];
		assert(displayModeList);

		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
		if (FAILED(hr))
		{
			m_window->showMessageBox("Query Refresh Rate", "Failed to query display mode list");
			return refreshRate;
		}

		// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
		for (UINT i = 0; i < numDisplayModes; ++i)
		{
			if (displayModeList[i].Width == m_window->getDimentions().x && displayModeList[i].Height == m_window->getDimentions().y)
			{
				refreshRate = displayModeList[i].RefreshRate;
			}
		}

		delete[] displayModeList;

		adapterOutput->Release(); adapterOutput = nullptr;
		adapter->Release(); adapter = nullptr;
		factory->Release(); factory = nullptr;
	}

	return refreshRate;
}

AGN::IAMesh* AGN::ADeviceDX11::createMesh(const uint16_t a_aId, AGN::AMeshData* a_meshData)
{
	return nullptr;
}

AGN::IATexture* AGN::ADeviceDX11::createTexture(const uint16_t a_aId, AGN::ATextureData* a_textureData)
{
	return nullptr;
}

AGN::IAShader* AGN::ADeviceDX11::createShader(const uint16_t a_aId, const char* a_shaderSource, AGN::EAShaderType a_type)
{
	return nullptr;
}

AGN::IAShaderPipeline* AGN::ADeviceDX11::createShaderPipeline(const uint16_t a_aId, std::vector<AGN::IAShader*> a_shaders)
{
	return nullptr;
}