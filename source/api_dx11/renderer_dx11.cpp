#include "shared.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <atlbase.h> // CCOMMpointers

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "renderer_dx11.hpp"
#include "application.hpp"
#include "window_dx11.hpp"
#include "render_api_dx11.hpp"
#include "mesh_dx11.hpp"
#include "texture_dx11.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "shader_pipeline_dx11.hpp"
#include "shader_dx11.hpp"
#include "device_dx11.hpp"
#include "draw_command.hpp"
#include "draw_commander.hpp"
#include "pixel_utils.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <imgui/imgui.h>

using namespace glm;

static const vec4 g_white(1);
static const vec4 g_black(0);

AGN::RendererDX11::RendererDX11(RenderAPIDX11& a_renderAPIReference, DeviceDX11& a_device, WindowDX11& a_window)
	: m_renderAPIReference(a_renderAPIReference)
	, m_deviceReference(a_device)
	, m_windowReference(a_window)
	, m_currentCamera(nullptr)
	, m_vp(mat4())
	, m_d3dRenderTargetView(nullptr)
	, m_d3dDepthStencilView(nullptr)
	, m_d3dDepthStencilBuffer(nullptr)
	, m_d3dDepthStencilState(nullptr)
	, m_d3dRasterizerState(nullptr)
	, m_d3dDefaultblendState(nullptr)
	, m_viewport(nullptr)
	, m_boundMesh(nullptr)
	, m_boundMaterial(nullptr)
	, m_boundShaderPipeline(nullptr)
{
}

AGN::RendererDX11::~RendererDX11()
{
	safeRelease(m_d3dRenderTargetView);
	safeRelease(m_d3dDepthStencilView);
	safeRelease(m_d3dDepthStencilBuffer);
	safeRelease(m_d3dDepthStencilState);
	safeRelease(m_d3dRasterizerState);
	safeRelease(m_d3dDefaultblendState);
	
	delete m_viewport;
}

bool AGN::RendererDX11::init()
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
	IDXGISwapChain* swapChain = m_deviceReference.getD3D11SwapChain();

	// get backbuffer 
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
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

	safeRelease(backBuffer);

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	memset(&depthStencilBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

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
	memset(&depthStencilStateDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = true;									// Set to TRUE to enable depth testing, or set to FALSE to disable depth testing.
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// Identify a portion of the depth-stencil buffer that can be modified by depth data // Turn on writes to the depth-stencil buffer.
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;					// compares depth data against existing depth data
	depthStencilStateDesc.StencilEnable = false;

	hr = d3d11Device->CreateDepthStencilState(&depthStencilStateDesc, &m_d3dDepthStencilState);

	if (FAILED(hr))
	{
		g_log.error("Failed creating Depth stencil state");
		return false;
	}

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = false;		// antialiasing; only applies if doing line drawing and MultisampleEnable is FALSE.
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;				// Enable clipping based on distance
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;			// Determines the fill mode to use when rendering // change to D3D11_FILL_WIREFRAME for wireframes
	rasterizerDesc.FrontCounterClockwise = true;		// Determines if a triangle is front- or back-facing
	rasterizerDesc.MultisampleEnable = false;			// MSAA
	rasterizerDesc.ScissorEnable = false;				// Enable scissor-rectangle culling
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

	// create default blend state
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		d3d11Device->CreateBlendState(&desc, &m_d3dDefaultblendState);
	}

	return true;
}

void AGN::RendererDX11::render(AGN::DrawCommander& a_drawCommander)
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

	/*
	g_log.debug("vp: %f - %f - %f - %f", m_vp[0][0], m_vp[0][1], m_vp[0][2], m_vp[0][3]);
	g_log.debug("vp: %f - %f - %f - %f", m_vp[1][0], m_vp[1][1], m_vp[1][2], m_vp[1][3]);
	g_log.debug("vp: %f - %f - %f - %f", m_vp[2][0], m_vp[2][1], m_vp[2][2], m_vp[2][3]);
	g_log.debug("vp: %f - %f - %f - %f", m_vp[3][0], m_vp[3][1], m_vp[3][2], m_vp[3][3]);
	*/

	setStaticStages();

	// loop through sorted draw commands & draw em
	std::vector<DrawCommand*> list = a_drawCommander.getSortedDrawCommands();

	for (unsigned int i = 0; i < list.size(); i++)
	{
		DrawCommand* command = list[i];

		switch (command->type)
		{
		case EDrawCommandType::ClearBuffer:
		{
			clearBuffer(*command);
			break;
		}
		case EDrawCommandType::DrawEntity:
		{
			drawEntity(*command);
			break;
		}
		case EDrawCommandType::DrawGUI:
		{
			renderGUI();
			break;
		}
		case EDrawCommandType::SwapBackBuffer:
		{
			if (m_renderAPIReference.getVSync())
			{
				m_deviceReference.getD3D11SwapChain()->Present(1, 0);
			}
			else
			{
				m_deviceReference.getD3D11SwapChain()->Present(0, 0);
			}
			break;
		}
		}
	}
}

void AGN::RendererDX11::drawEntity(DrawCommand& a_command)
{
	ID3D11DeviceContext* const d3dDeviceContext = m_deviceReference.getD3D11DeviceContext();

	DrawEntityData& data = a_command.data.entityData;
	MeshDX11* mesh = dynamic_cast<MeshDX11*>(data.mesh);
	Material* material = dynamic_cast<Material*>(data.material);
	ShaderPipelineDX11* shaderPipeline = dynamic_cast<ShaderPipelineDX11*>(data.shaderPipeline);

	m_deviceReference.beginDebugEvent("Render Mesh: " + mesh->getRelativePath());

	// input assembler stage
	if (m_boundMesh == nullptr || m_boundMesh->getAId() != mesh->getAId())
	{
		m_boundMesh = mesh;
		
		// get vertex stride (inputdata size)
		// TODO: get this from shader parsing(reflection) not hardcoded!
		const uint32_t vertexStride = sizeof(MeshDX11::VertexShaderData);
		const uint32_t offset = 0;

		ID3D11Buffer* vertexBuffer = mesh->getD3D11VertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->getD3D11IndexBuffer();

		d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);
		d3dDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// Vertex shader stage
	if (m_boundShaderPipeline == nullptr || m_boundShaderPipeline->getAId() != shaderPipeline->getAId())
	{
		m_boundShaderPipeline = shaderPipeline;

		// set new vertex shader
		{
			ShaderDX11* dx11VertexShader = dynamic_cast<ShaderDX11*>(shaderPipeline->getShader(EShaderType::VertexShader));
			ID3D11VertexShader* d3d11VertexShader;
			if (FAILED(dx11VertexShader->getD3D11Shader()->QueryInterface(IID_ID3D11VertexShader, (void**)&d3d11VertexShader)))
			{
				assert(false);
			}

			d3dDeviceContext->VSSetShader(d3d11VertexShader, nullptr, 0);

			safeRelease(d3d11VertexShader);
		}

		// pixel shader stage
		{
			ShaderDX11* dx11PixelShader = dynamic_cast<ShaderDX11*>(shaderPipeline->getShader(EShaderType::PixelShader));

			ID3D11PixelShader* d3d11PixelShader;
			if (SUCCEEDED(dx11PixelShader->getD3D11Shader()->QueryInterface(IID_ID3D11PixelShader, (void**)&d3d11PixelShader)))
			{
				d3dDeviceContext->PSSetShader(d3d11PixelShader, nullptr, 0);
			}

			// set constant buffer
			const int numConstBuffers = dx11PixelShader->getNumConstantBuffers();

			if (numConstBuffers > 0)
			{
				ID3D11Buffer** d3d11ConstantBuffers = dx11PixelShader->getConstantBufferHandles();			
				d3dDeviceContext->PSSetConstantBuffers(0, numConstBuffers, d3d11ConstantBuffers);
			}

			// set PS sampler
			ID3D11SamplerState* sampler = shaderPipeline->getSamplerState();
			d3dDeviceContext->PSSetSamplers(0, 1, &sampler);

			safeRelease(d3d11PixelShader);
		}

		// inputlayout
		ID3D11InputLayout* vertexInputLayout = shaderPipeline->getVertexInputLayout();
		d3dDeviceContext->IASetInputLayout(vertexInputLayout);

	}

	// set material data
	if (m_boundMaterial == nullptr || m_boundMaterial->getAId() != material->getAId())
	{
		m_boundMaterial = material;

		// set material constant buffer
		// TODO: move to only when material changes!
		if (shaderPipeline->hasConstantBuffer(EShaderType::PixelShader, "MaterialProperties"))
		{
			
			ConstantBufferDX11* materialBuffer = shaderPipeline->getConstantBufferByName(EShaderType::PixelShader, "MaterialProperties");
			
			// TODO: get buffer offset, this is hardcoded
			uint8_t* buffer = new uint8_t[materialBuffer->size]; // TODO: optimize
			memset(buffer, 0, materialBuffer->size);
			
			ConstantBufferPropertyDX11* transparency = materialBuffer->getPropertyByName("materialTransparency");
			ConstantBufferPropertyDX11* diffuse = materialBuffer->getPropertyByName("materialDiffuseColor");
			ConstantBufferPropertyDX11* ambient = materialBuffer->getPropertyByName("materialAmbientColor");
			
			memcpy(buffer + transparency->offset, &material->transparency, sizeof(material->transparency));
			memcpy(buffer + diffuse->offset, glm::value_ptr(material->diffuseColor), sizeof(material->diffuseColor));
			memcpy(buffer + ambient->offset, glm::value_ptr(material->ambientColor), sizeof(material->ambientColor));
			//memcpy(buffer + 32, glm::value_ptr(material->specularColor), sizeof(glm::vec3)); // material specular
			
			shaderPipeline->setConstantBufferData(EShaderType::PixelShader, "MaterialProperties", buffer, sizeof(buffer));
			
			delete[] buffer; // TODO: optimize
			
		}

		// set textures
		TextureDX11* diffuse = dynamic_cast<TextureDX11*>(material->diffuseTexture);
		TextureDX11* normal = dynamic_cast<TextureDX11*>(material->normalTexture);
		TextureDX11* specular = dynamic_cast<TextureDX11*>(material->specularTexture);

		// TODO: add multiple texture support
		if (diffuse != nullptr)
		{
			ID3D11ShaderResourceView* shaderResourceView = diffuse->getShaderResourceView();
			d3dDeviceContext->PSSetShaderResources(0, 1, &shaderResourceView);
		}

	}

	// set vertex shader data (different per object
	{
		ShaderDX11* aDx11VertexShader = dynamic_cast<ShaderDX11*>(shaderPipeline->getShader(EShaderType::VertexShader));
		ID3D11VertexShader* d3d11VertexShader;
		if (FAILED(aDx11VertexShader->getD3D11Shader()->QueryInterface(IID_ID3D11VertexShader, (void**)&d3d11VertexShader)))
		{
			assert(false);
		}

		// prepare Vertex input data (MVP)
		mat4 modelMatrix = glm::make_mat4(data.modelMatrixArray); 		// retrieve model matrix from array in struct
		mat4 mvp = m_vp * modelMatrix;

		unsigned char buffer[128] = { 0 };
		memcpy(buffer, glm::value_ptr(mvp), sizeof(mvp));
		memcpy(buffer + sizeof(mvp), glm::value_ptr(modelMatrix), sizeof(modelMatrix));

		aDx11VertexShader->setConstantBufferData("PerObject", &buffer, sizeof(buffer));

		const int numConstBuffers = aDx11VertexShader->getNumConstantBuffers();
		if (numConstBuffers > 0)
		{
			ID3D11Buffer** d3d11ConstantBuffers = aDx11VertexShader->getConstantBufferHandles();
			d3dDeviceContext->VSSetConstantBuffers(0, numConstBuffers, d3d11ConstantBuffers);
		}

		safeRelease(d3d11VertexShader);
	}

	// render the mesh
	d3dDeviceContext->DrawIndexed((uint32_t)mesh->getIndexCount(), 0, 0);

	m_deviceReference.endDebugEvent();
}

void AGN::RendererDX11::clearBuffer(DrawCommand& a_command)
{
	if (a_command.data.clearBufferData.buffersToClear & (uint32_t)EDrawBufferType::COLOR)
	{
		float clearColorFloat[4] = { 0.0f };

		AGN::PixelUtils::getRGBAFloat(
			a_command.data.clearBufferData.clearColor,
			clearColorFloat[0],
			clearColorFloat[1],
			clearColorFloat[2],
			clearColorFloat[3]);

		m_deviceReference.getD3D11DeviceContext()->ClearRenderTargetView(m_d3dRenderTargetView, clearColorFloat);
	}
	
	if (a_command.data.clearBufferData.buffersToClear & (uint32_t)EDrawBufferType::DEPTH)
	{
		m_deviceReference.getD3D11DeviceContext()->ClearDepthStencilView(m_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

}

/*
	Set the stages that do not change each frame
	// TODO: perhaps change these up to stages that do not change at all ever and each frame?
*/
void AGN::RendererDX11::setStaticStages()
{
	ID3D11DeviceContext* const d3dDeviceContext = m_deviceReference.getD3D11DeviceContext();

	// input assembler stage
	{
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	// rasterizer Stage
	{
		d3dDeviceContext->RSSetState(m_d3dRasterizerState);
		d3dDeviceContext->RSSetViewports(1, m_viewport);
	}

	// output merger stage
	{
		d3dDeviceContext->OMSetRenderTargets(1, &m_d3dRenderTargetView, m_d3dDepthStencilView);
		d3dDeviceContext->OMSetDepthStencilState(m_d3dDepthStencilState, 1);
	}

	// blend state
	{
		const float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		d3dDeviceContext->OMSetBlendState(m_d3dDefaultblendState, blendFactor, 0xffffffff);
	}

}

void AGN::RendererDX11::renderGUI()
{
	m_deviceReference.beginDebugEvent("Render ImGui");

	ID3D11DeviceContext* const d3dDeviceContext = m_deviceReference.getD3D11DeviceContext();

	// disable depth checking on render target for gui
	d3dDeviceContext->OMSetRenderTargets(1, &m_d3dRenderTargetView, nullptr);	

	ImGui::Render();

	// enable depth checking again
	d3dDeviceContext->OMSetRenderTargets(1, &m_d3dRenderTargetView, m_d3dDepthStencilView);	
	m_deviceReference.endDebugEvent();
}

void AGN::RendererDX11::onWindowUpdated(glm::ivec2 a_dimensions)
{
	g_log.debug("ARendererDX11::onWindowUpdated()");

	HRESULT hr;

	m_deviceReference.getD3D11DeviceContext()->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	safeRelease(m_d3dRenderTargetView);

	// Preserve the existing buffer count and format.
	// Automatically choose the width and height to match the client rect for HWNDs.
	hr = m_deviceReference.getD3D11SwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	if (FAILED(hr))
	{
		g_log.error("m_d3dSwapChain ResizeBuffers failure");
		return;
	}

	safeRelease(m_d3dDepthStencilView);
	safeRelease(m_d3dDepthStencilBuffer);

	// re-create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	memset(&depthStencilBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;									// Number of textures in the texture array
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// how to bind a resource to the pipeline // indicates that this buffer is to be used as a depth-stencil target for the output-merger stage.
	depthStencilBufferDesc.CPUAccessFlags = 0;								// No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// a 32-bit z-buffer format that supports 24 bits for depth and 8 bits for stencil.
	depthStencilBufferDesc.Width = a_dimensions.x;
	depthStencilBufferDesc.Height = a_dimensions.y;
	depthStencilBufferDesc.MipLevels = 1;									// The maximum number of mipmap levels in the texture // 1 for a multisampled texture; or 0 to generate a full set of subtextures.
	depthStencilBufferDesc.SampleDesc.Count = 1;							// multisampling parameters for the texture. 
	depthStencilBufferDesc.SampleDesc.Quality = 0;							// multisampling parameters for the texture. 
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;						// w the texture is to be read from and written to

	hr = m_deviceReference.getD3D11Device()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_d3dDepthStencilBuffer);

	if (FAILED(hr))
	{
		g_log.error("m_d3dDevice->CreateTexture2D() failure");
		return;
	}

	hr = m_deviceReference.getD3D11Device()->CreateDepthStencilView(m_d3dDepthStencilBuffer, nullptr, &m_d3dDepthStencilView);
	if (FAILED(hr))
	{
		g_log.error("CreateDepthStencilView failure");
		return;
	}

	// Get buffer and create a render-target-view.
	ID3D11Texture2D* pBuffer;
	hr = m_deviceReference.getD3D11SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

	if (FAILED(hr))
	{
		g_log.error("m_d3dSwapChain GetBuffer failure");
		return;
	}

	hr = m_deviceReference.getD3D11Device()->CreateRenderTargetView(pBuffer, nullptr, &m_d3dRenderTargetView);

	if (FAILED(hr))
	{
		g_log.error("m_d3dDevice CreateRenderTargetView failure");
		return;
	}

	safeRelease(pBuffer);

	m_deviceReference.getD3D11DeviceContext()->OMSetRenderTargets(1, &m_d3dRenderTargetView, nullptr);

	// Set up the viewport.
	m_viewport->Width = static_cast<float>(a_dimensions.x);
	m_viewport->Height = static_cast<float>(a_dimensions.y);
	m_viewport->TopLeftX = 0.0f;
	m_viewport->TopLeftY = 0.0f;
	m_viewport->MinDepth = 0.0f;
	m_viewport->MaxDepth = 1.0f;

	m_deviceReference.getD3D11DeviceContext()->RSSetViewports(1, m_viewport);
}
