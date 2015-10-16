#include "asharedh.hpp"
#include "arenderer_dx11.hpp"
#include "aaplication.hpp"
#include "awindow_dx11.hpp"
#include "arender_api_dx11.hpp"
#include "amesh_dx11.hpp"
#include "atexture_dx11.hpp"
#include "amaterial.hpp"
#include "acamera.hpp"
#include "aentity.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "adevice_dx11.hpp"
#include "adrawcommand.hpp"
#include "adrawcommander.hpp"
#include "apixelutils.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);

AGN::ARendererDX11::ARendererDX11(ARenderAPIDX11& a_renderAPIReference, ADeviceDX11& a_device, AWindowDX11& a_window)
	: m_renderAPIReference(a_renderAPIReference)
	, m_deviceReference(a_device)
	, m_windowReference(a_window)
	, m_currentCamera(nullptr)
	, m_vp(mat4())
	, m_boundMesh(nullptr)
	, m_boundMaterial(nullptr)
	, m_boundShaderPipeline(nullptr)
	, m_d3dRenderTargetView(nullptr)
	, m_d3dDepthStencilView(nullptr)
	, m_d3dDepthStencilBuffer(nullptr)
	, m_d3dDepthStencilState(nullptr)
	, m_d3dRasterizerState(nullptr)
	, m_viewport(nullptr)
{

}

bool AGN::ARendererDX11::init()
{
	ID3D11Device* d3d11Device = m_deviceReference.getD3D11Device();

	RECT clientRect;
	GetClientRect(m_windowReference.getWindowHandle(), &clientRect);

	// Compute the exact client dimensions. This will be used
	// to initialize the render targets for our swap chain.
	unsigned int clientWidth = clientRect.right - clientRect.left;
	unsigned int clientHeight = clientRect.bottom - clientRect.top;

	// initialize the back buffer of the swap chain and associate it to a render target view.
	ID3D11Texture2D* backBuffer;
	HRESULT hr = m_deviceReference.getD3D11SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		g_log.error("d3dSwapChain->GetBuffer() Failure");
		return false;
	}

	hr = d3d11Device->CreateRenderTargetView(
		backBuffer,									// represents a render target
		nullptr,									// a render-target view description (NULL to create a view that accesses all of the subresources in mipmap level 0.)
		&m_d3dRenderTargetView);

	if (FAILED(hr))
	{
		g_log.error("m_d3dDevice->CreateRenderTargetView() Failure");
		return false;
	}

	backBuffer->Release();
	backBuffer = nullptr;

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;									// Number of textures in the texture array
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// how to bind a resource to the pipeline // indicates that this buffer is to be used as a depth-stencil target for the output-merger stage.
	depthStencilBufferDesc.CPUAccessFlags = 0;								// No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// a 32-bit z-buffer format that supports 24 bits for depth and 8 bits for stencil.
	depthStencilBufferDesc.Width = clientWidth;
	depthStencilBufferDesc.Height = clientHeight;
	depthStencilBufferDesc.MipLevels = 1;									// The maximum number of mipmap levels in the texture // 1 for a multisampled texture; or 0 to generate a full set of subtextures.
	depthStencilBufferDesc.SampleDesc.Count = 1;							// multisampling parameters for the texture. 
	depthStencilBufferDesc.SampleDesc.Quality = 0;							// multisampling parameters for the texture. 
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;						// w the texture is to be read from and written to

	hr = d3d11Device->CreateTexture2D(
		&depthStencilBufferDesc,			// describes a 2D texture resource
		nullptr,							// describe subresources for the 2D texture resource //  must be NULL because multisampled resources cannot be initialized with data when they are created
		&m_d3dDepthStencilBuffer);

	if (FAILED(hr))
	{
		g_log.error("m_d3dDevice->CreateTexture2D() failure");
		return false;
	}

	hr = d3d11Device->CreateDepthStencilView(m_d3dDepthStencilBuffer, nullptr, &m_d3dDepthStencilView);
	if (FAILED(hr))
	{
		g_log.error("CreateDepthStencilView failure");
		return false;
	}

	// Setup depth/stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;									// Set to TRUE to enable depth testing, or set to FALSE to disable depth testing.
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// Identify a portion of the depth-stencil buffer that can be modified by depth data // Turn on writes to the depth-stencil buffer.
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;					// compares depth data against existing depth data
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = d3d11Device->CreateDepthStencilState(&depthStencilStateDesc, &m_d3dDepthStencilState);

	if (FAILED(hr))
	{
		g_log.error("Failed creating Depth stencil state");
		return false;
	}

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;		// antialiasing; only applies if doing line drawing and MultisampleEnable is FALSE.
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;				// Enable clipping based on distance
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;			// Determines the fill mode to use when rendering // change to D3D11_FILL_WIREFRAME for wireframes
	rasterizerDesc.FrontCounterClockwise = TRUE;		// Determines if a triangle is front- or back-facing
	rasterizerDesc.MultisampleEnable = FALSE;			// MSAA
	rasterizerDesc.ScissorEnable = FALSE;				// Enable scissor-rectangle culling
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	hr = d3d11Device->CreateRasterizerState(&rasterizerDesc, &m_d3dRasterizerState);
	if (FAILED(hr))
	{
		g_log.error("CreateRasterizerState Failure");
		return false;
	}

	// Initialize the viewport to occupy the entire client area.
	m_viewport = new D3D11_VIEWPORT();
	m_viewport->Width = static_cast<float>(clientWidth);
	m_viewport->Height = static_cast<float>(clientHeight);
	m_viewport->TopLeftX = 0.0f;
	m_viewport->TopLeftY = 0.0f;
	m_viewport->MinDepth = 0.0f;
	m_viewport->MaxDepth = 1.0f;

	return true;
}

void AGN::ARendererDX11::render(AGN::ADrawCommander& a_drawCommander)
{
	if (m_currentCamera == nullptr)
	{
		g_log.error("Camera was not set, cannot render!");
		return;
	}

	m_boundMesh = nullptr;
	m_boundMaterial = nullptr;
	m_boundShaderPipeline = nullptr;

	// make sure the camera has the correct ViewSettings
	m_currentCamera->setProjectionRH(60.0f, 0.1f, 10000.0f);

	// pre-calculate matrices that we are going to re-use through the frame
	m_vp = m_currentCamera->getProjectionMatrix() * m_currentCamera->getViewMatrix();

	// loop through sorted draw commands & draw em
	std::vector<ADrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (unsigned int i = 0; i < list.size(); i++)
	{
		ADrawCommand* command = list[i];

		switch (command->type)
		{
		case EADrawCommandType::ClearBuffer:
			clearBuffer(*command);
			break;

		case EADrawCommandType::DrawEntity:
			drawEntity(*command);
			break;

		case EADrawCommandType::SwapBackBuffer:

			IDXGISwapChain* d3d11SwapChain = m_deviceReference.getD3D11SwapChain();
			
			if (m_renderAPIReference.getVSync())
			{
				d3d11SwapChain->Present(1, 0);
			}
			else
			{
				d3d11SwapChain->Present(0, 0);
			}
			break;
		}
	}
}

void AGN::ARendererDX11::drawEntity(ADrawCommand& a_command)
{
	// TODO:

}

void AGN::ARendererDX11::clearBuffer(struct ADrawCommand& a_command)
{
	if (a_command.data.clearBufferData.buffersToClear & (uint32_t)ADrawBufferType::COLOR)
	{
		float clearColorFloat[4] = { 0.0f };

		AGN::PixelUtils::getARGBFloat(
			a_command.data.clearBufferData.clearColor,
			clearColorFloat[0],
			clearColorFloat[1],
			clearColorFloat[2],
			clearColorFloat[3]);

		m_deviceReference.getD3D11DeviceContext()->ClearRenderTargetView(m_d3dRenderTargetView, clearColorFloat);
	}
	
	if (a_command.data.clearBufferData.buffersToClear & (uint32_t)ADrawBufferType::DEPTH)
	{
		m_deviceReference.getD3D11DeviceContext()->ClearDepthStencilView(m_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
	}

	


	//AClearBufferData& data = drawCommand.data.clearBufferData;
	//data.buffersToClear = (uint32_t)ADrawBufferType::COLOR | (uint32_t)ADrawBufferType::DEPTH;
	//data.clearColor = 0x00FF00;

}

