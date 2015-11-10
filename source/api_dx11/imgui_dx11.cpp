#include "shared.hpp"
#include "imgui_dx11.hpp"
#include "render_api_dx11.hpp"
#include "window_dx11.hpp"
#include "i_input.hpp"
#include "device_dx11.hpp"

#include <imgui/imgui.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

static AGN::ImGuiDX11* g_instance = nullptr; // TODO: refactor

struct VERTEX_CONSTANT_BUFFER
{
	float mvp[4][4];
};

void AGN::ImGuiDX11::render(ImDrawData* draw_data)
{
	DeviceDX11& deviceDX11 = dynamic_cast<DeviceDX11&>(m_renderAPI->getDevice());
	ID3D11Device* d3dDevice = deviceDX11.getD3D11Device();
	ID3D11DeviceContext* d3dDeviceContext = deviceDX11.getD3D11DeviceContext();

	// Create and grow vertex/index buffers if needed
	if (!m_vertexBuffer || m_vertexBufferSize < draw_data->TotalVtxCount)
	{
		if (m_vertexBuffer) { m_vertexBuffer->Release(); m_vertexBuffer = nullptr; }
		m_vertexBufferSize = draw_data->TotalVtxCount + 5000;
		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = m_vertexBufferSize * sizeof(ImDrawVert);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		if (d3dDevice->CreateBuffer(&bufferDesc, nullptr, &m_vertexBuffer) < 0)
		{
			g_log.error("Failure during rendering GUIDX11");
			return;
		}
	}
	if (!m_indexBuffer || m_indexBufferSize < draw_data->TotalIdxCount)
	{
		if (m_indexBuffer) { m_indexBuffer->Release(); m_indexBuffer = nullptr; }
		m_indexBufferSize = draw_data->TotalIdxCount + 10000;
		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = m_indexBufferSize * sizeof(ImDrawIdx);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (d3dDevice->CreateBuffer(&bufferDesc, nullptr, &m_indexBuffer) < 0)
		{
			g_log.error("Failure during rendering GUIDX11");
			return;
		}
	}

	// Copy and convert all vertices into a single contiguous buffer
	D3D11_MAPPED_SUBRESOURCE vtxResource, idxResource;
	if (d3dDeviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtxResource) != S_OK)
	{
		g_log.error("Failure during rendering GUIDX11");
		return;
	}

	if (d3dDeviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idxResource) != S_OK)
	{
		g_log.error("Failure during rendering GUIDX11");
		return;
	}
	ImDrawVert* vtxDst = (ImDrawVert*)vtxResource.pData;
	ImDrawIdx* idxDst = (ImDrawIdx*)idxResource.pData;
	for (int32_t n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		memcpy(vtxDst, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
		memcpy(idxDst, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));
		vtxDst += cmd_list->VtxBuffer.size();
		idxDst += cmd_list->IdxBuffer.size();
	}
	d3dDeviceContext->Unmap(m_vertexBuffer, 0);
	d3dDeviceContext->Unmap(m_indexBuffer, 0);

	// Setup orthographic projection matrix into our constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (d3dDeviceContext->Map(m_vertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
		{
			g_log.error("Failure during rendering GUIDX11");
			return;
		}

		VERTEX_CONSTANT_BUFFER* pConstantBuffer = (VERTEX_CONSTANT_BUFFER*)mappedResource.pData;
		const float L = 0.0f;
		const float R = ImGui::GetIO().DisplaySize.x;
		const float B = ImGui::GetIO().DisplaySize.y;
		const float T = 0.0f;
		const float mvp[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f, },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
		};
		memcpy(&pConstantBuffer->mvp, mvp, sizeof(mvp));
		d3dDeviceContext->Unmap(m_vertexConstantBuffer, 0);
	}

	/*
	// Setup viewport
	{
		D3D11_VIEWPORT vp;
		memset(&vp, 0, sizeof(D3D11_VIEWPORT));
		vp.Width = ImGui::GetIO().DisplaySize.x;
		vp.Height = ImGui::GetIO().DisplaySize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		d3dDeviceContext->RSSetViewports(1, &vp);
	}
	*/

	// Bind shader and vertex buffers
	uint32_t stride = sizeof(ImDrawVert);
	uint32_t offset = 0;
	d3dDeviceContext->IASetInputLayout(m_inputLayout);
	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_vertexConstantBuffer);
	d3dDeviceContext->PSSetShader(m_pixelShader, nullptr, 0);
	d3dDeviceContext->PSSetSamplers(0, 1, &m_fontSampler);

	// Setup render state
	const float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	d3dDeviceContext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);
	d3dDeviceContext->RSSetState(m_rasterizerState);

	// Render command lists
	int32_t vtxOffset = 0;
	int32_t idxOffset = 0;
	for (int32_t n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmdList = draw_data->CmdLists[n];
		for (int32_t cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.size(); cmdIndex++)
		{
			const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmdIndex];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmdList, pcmd);
			}
			else
			{
				const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
				d3dDeviceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
				d3dDeviceContext->RSSetScissorRects(1, &r);
				d3dDeviceContext->DrawIndexed(pcmd->ElemCount, idxOffset, vtxOffset);
			}
			idxOffset += pcmd->ElemCount;
		}
		vtxOffset += cmdList->VtxBuffer.size();
	}

	// Restore modified state
	d3dDeviceContext->IASetInputLayout(nullptr);
	d3dDeviceContext->PSSetShader(nullptr, nullptr, 0);
	d3dDeviceContext->VSSetShader(nullptr, nullptr, 0);
	d3dDeviceContext->RSSetScissorRects(0,nullptr);
}

#if defined(__GNUC__) || defined(__GNUG__)
// static friend functions are not supported in GCC
void renderDrawLists(ImDrawData* draw_data)
{
	g_instance->render(draw_data);
}
#else
void AGN::renderDrawLists(ImDrawData* draw_data)
{
	g_instance->render(draw_data);
}
#endif

/*
IMGUI_API LRESULT ImGui_ImplDX11_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}
	return 0;
}*/

void AGN::ImGuiDX11::createDeviceObjects()
{
	if (m_fontSampler) invalidateDeviceObjects();

	DeviceDX11& deviceDX11 = dynamic_cast<DeviceDX11&>(m_renderAPI->getDevice());
	ID3D11Device* d3dDevice = deviceDX11.getD3D11Device();
	HRESULT hr = S_OK;

	// Create the vertex shader
	{
		static const char* vertexShader =
			"cbuffer vertexBuffer : register(b0) \
			{\
			float4x4 ProjectionMatrix; \
			};\
			struct VS_INPUT\
			{\
			float2 pos : POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			\
			struct PS_INPUT\
			{\
			float4 pos : SV_POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			\
			PS_INPUT main(VS_INPUT input)\
			{\
			PS_INPUT output;\
			output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
			output.col = input.col;\
			output.uv  = input.uv;\
			return output;\
			}";

		hr = D3DCompile(vertexShader, strlen(vertexShader), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &m_vertexShaderBlob, nullptr);
		if (m_vertexShaderBlob == nullptr)
		{
			// NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
			g_log.error("Failure during creation of GUIDX11 device objects");
			return;
		}

		if (d3dDevice->CreateVertexShader((DWORD*)m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), nullptr, &m_vertexShader) != S_OK)
		{
			g_log.error("Failure during creation of GUIDX11 device objects");
			return;
		}

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC localLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		if (d3dDevice->CreateInputLayout(localLayout, 3, m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), &m_inputLayout) != S_OK)
		{
			g_log.error("Failure during creation of GUIDX11 device objects");
			return;
		}

		// Create the constant buffer
		{
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			hr = d3dDevice->CreateBuffer(&cbDesc, nullptr, &m_vertexConstantBuffer);
		}
	}

	// Create the pixel shader
	{
		static const char* pixelShader =
			"struct PS_INPUT\
			{\
			float4 pos : SV_POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			sampler sampler0;\
			Texture2D texture0;\
			\
			float4 main(PS_INPUT input) : SV_Target\
			{\
			float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
			return out_col; \
			}";

		hr = D3DCompile(pixelShader, strlen(pixelShader), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &m_pixelShaderBlob, nullptr);
		if (m_pixelShaderBlob == nullptr)
		{
			// NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
			g_log.error("Failure during creation of GUIDX11 device objects");
			return;
		}
		if (d3dDevice->CreatePixelShader((DWORD*)m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize(), nullptr, &m_pixelShader) != S_OK)
		{
			g_log.error("Failure during creation of GUIDX11 device objects");
			return;
		}
	}

	// Create the blending setup
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = d3dDevice->CreateBlendState(&blendDesc, &m_blendState);
	}

	// Create the rasterizer state
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.ScissorEnable = true;
		rasterizerDesc.DepthClipEnable = true;
		hr = d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	}

	createImGUIFont();
}

void AGN::ImGuiDX11::createImGUIFont()
{
	ImGuiIO& io = ImGui::GetIO();

	DeviceDX11& deviceDX11 = dynamic_cast<DeviceDX11&>(m_renderAPI->getDevice());
	ID3D11Device* d3dDevice = deviceDX11.getD3D11Device();

	// Build
	HRESULT hr = S_OK;
	unsigned char* pixels;
	int32_t width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	// Create DX11 texture
	{
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;

		ID3D11Texture2D *pTexture = nullptr;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = texDesc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		hr = d3dDevice->CreateTexture2D(&texDesc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &m_fontTextureView);
		pTexture->Release();
	}

	// Store our identifier
	io.Fonts->TexID = (void *)m_fontTextureView;

	// Create texture sampler
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.f;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0.f;
		samplerDesc.MaxLOD = 0.f;
		hr = d3dDevice->CreateSamplerState(&samplerDesc, &m_fontSampler);
	}

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
}

void AGN::ImGuiDX11::invalidateDeviceObjects()
{
	if (m_fontSampler) { m_fontSampler->Release(); m_fontSampler = nullptr; }
	if (m_fontTextureView) { m_fontTextureView->Release(); m_fontTextureView = nullptr; ImGui::GetIO().Fonts->TexID = 0; }
	if (m_indexBuffer) { m_indexBuffer->Release(); m_indexBuffer = nullptr; }
	if (m_vertexBuffer) { m_vertexBuffer->Release(); m_vertexBuffer = nullptr; }

	if (m_blendState) { m_blendState->Release(); m_blendState = nullptr; }
	if (m_rasterizerState) { m_rasterizerState->Release(); m_rasterizerState = nullptr; }
	if (m_pixelShader) { m_pixelShader->Release(); m_pixelShader = nullptr; }
	if (m_pixelShaderBlob) { m_pixelShaderBlob->Release(); m_pixelShaderBlob = nullptr; }
	if (m_vertexConstantBuffer) { m_vertexConstantBuffer->Release(); m_vertexConstantBuffer = nullptr; }
	if (m_inputLayout) { m_inputLayout->Release(); m_inputLayout = nullptr; }
	if (m_vertexShader) { m_vertexShader->Release(); m_vertexShader = nullptr; }
	if (m_vertexShaderBlob) { m_vertexShaderBlob->Release(); m_vertexShaderBlob = nullptr; }
}

AGN::ImGuiDX11::ImGuiDX11()
	: m_isEnabled(false)
	, m_renderAPI(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_vertexShaderBlob(nullptr)
	, m_vertexShader(nullptr)
	, m_inputLayout(nullptr)
	, m_vertexConstantBuffer(nullptr)
	, m_pixelShaderBlob(nullptr)
	, m_pixelShader(nullptr)
	, m_fontSampler(nullptr)
	, m_fontTextureView(nullptr)
	, m_rasterizerState(nullptr)
	, m_blendState(nullptr)
	, m_vertexBufferSize(5000)
	, m_indexBufferSize(10000)
{
	if (g_instance != nullptr) assert(false); // ensure only one gui can exist // TODO: refactor
	g_instance = this;
}

void AGN::ImGuiDX11::init(RenderAPIDX11* a_api)
{
	m_renderAPI = a_api;
	m_isEnabled = true;
	
	WindowDX11& windowDX11 = dynamic_cast<WindowDX11&>(a_api->getWindow());
	ImGuiIO& io = ImGui::GetIO();
	io.RenderDrawListsFn = renderDrawLists;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer	
	io.ImeWindowHandle = windowDX11.getWindowHandle();

	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';
}

void AGN::ImGuiDX11::shutdown()
{
	invalidateDeviceObjects();
	ImGui::Shutdown();
}

void AGN::ImGuiDX11::update(float a_deltaTime)
{
	if (!m_vertexBuffer) createDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = a_deltaTime;

	// Setup display size (every frame to accommodate for window resizing)
	// TODO: change when window resizes?
	const glm::ivec2 windowDimentions = m_renderAPI->getWindow().getDimentions();
	io.DisplaySize = ImVec2(static_cast<float>(windowDimentions.x), static_cast<float>(windowDimentions.y));

	// set inputs
	io.MousePos = ImVec2(static_cast<float>(g_input.getMouseX()), static_cast<float>(g_input.getMouseY()));
	io.MouseDown[0] = g_input.getMouse(EMOUSECODE::LEFT);
	io.MouseDown[1] = g_input.getMouse(EMOUSECODE::RIGHT);
	io.MouseDown[2] = g_input.getMouse(EMOUSECODE::MIDDLE);

	if (g_input.getMouseScroll() > 0) io.MouseWheel = 1;
	else if (g_input.getMouseScroll() < 0) io.MouseWheel = -1;
	else io.MouseWheel = 0;

	// Hide OS mouse cursor if ImGui is drawing it
	SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));

	// Start the frame
	ImGui::NewFrame();
}