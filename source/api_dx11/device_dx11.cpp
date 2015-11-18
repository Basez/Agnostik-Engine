#include "shared.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <string.h>
#include <comdef.h>		// TODO: remove used for error checking


static void logHResultData(HRESULT a_hr)
{
	_com_error error(a_hr);
	g_log.debug("HRESULT Message: %s", error.ErrorMessage());
	g_log.debug("HRESULT Description: %s", error.Description());
}

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "device_dx11.hpp"
#include "mesh_dx11.hpp"
#include "texture_dx11.hpp"
#include "shader_dx11.hpp"
#include "shader_pipeline_dx11.hpp"
#include "window_dx11.hpp"
#include "config_manager.hpp"
#include "render_api_dx11.hpp"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"


using namespace glm;

AGN::DeviceDX11::DeviceDX11()
	: m_window(nullptr)
	, m_d3d11Device(nullptr)
	, m_d3d11DeviceContext(nullptr)
	, m_d3d11SwapChain(nullptr)
	, m_d3dDebug(nullptr)
	, m_debugUDA(nullptr)
	, m_MSAALevel(1)
	, m_MSAAQuality(0)
{

}

AGN::DeviceDX11::~DeviceDX11()
{
	safeRelease(m_d3d11Device);
	safeRelease(m_d3d11DeviceContext);
	safeRelease(m_d3d11SwapChain);

	logLiveObjects();

	safeRelease(m_d3dDebug);
	safeRelease(m_debugUDA);
}

bool AGN::DeviceDX11::init(class WindowDX11* a_window)
{	
	m_window = a_window;

	assert(m_window);
	assert(m_window->getWindowHandle());

	// initialize device without AA enabled (so we can query it)
	cleanAndInitializeDevice(1, 0);

	glm::ivec2 maxSampleQuality = getMaxMSAASampleQuality();

	// initialize with AA enabled
	if (maxSampleQuality.x != 1)
	{
		cleanAndInitializeDevice(maxSampleQuality.x, maxSampleQuality.y);
		g_log.debug("MSAA enabled with sample size of %i and a quality of %i", maxSampleQuality.x, maxSampleQuality.y);
	}
	else
	{
		g_log.warning("MSAA not available on this device...");
	}

	// get debug interface
#ifdef AGN_DEBUG

	m_d3dDebug = nullptr;
	if (SUCCEEDED(m_d3d11Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_d3dDebug)))
	{
		ID3D11InfoQueue *d3dInfoQueue = nullptr;
		if (SUCCEEDED(m_d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
		{
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// Add more message IDs here as needed
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
			safeRelease(d3dInfoQueue);
		}
	}

	if (FAILED(m_d3d11DeviceContext->QueryInterface(IID_PPV_ARGS(&m_debugUDA))))
	{
		g_log.error("Failure getting debugUDA");
		assert(false);
		return false;
	}
#endif

	return true;
}

void AGN::DeviceDX11::cleanAndInitializeDevice(int a_MSAALevel, int a_MSAAQuality)
{
	safeRelease(m_d3d11Device);
	safeRelease(m_d3d11DeviceContext);
	safeRelease(m_d3d11SwapChain);

	m_MSAALevel = a_MSAALevel;
	m_MSAAQuality = a_MSAAQuality;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	bool vsync = g_configManager.getConfigPropertyAsBool("vsync");

	swapChainDesc.BufferCount = 2;												// the number of buffers in the swap chain
	swapChainDesc.BufferDesc.Width = static_cast<unsigned int>(m_window->getDimensions().x);
	swapChainDesc.BufferDesc.Height = static_cast<unsigned int>(m_window->getDimensions().y);
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// pixel format of the display in this case: a 4-component 32-bit unsigned normalized integer format that supports 8 bits per channel including alpha
	swapChainDesc.BufferDesc.RefreshRate = queryRefreshRate(vsync);				// refresh rate in hertz, 0/1 to specify an unbounded refresh rate.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;				// describes the surface usage and CPU access options for the back buffer in this case: Use the surface or resource as an output render target.
	swapChainDesc.OutputWindow = m_window->getWindowHandle();
	swapChainDesc.SampleDesc.Count = a_MSAALevel;
	swapChainDesc.SampleDesc.Quality = a_MSAAQuality;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;						// describes options for handling the contents of the presentation buffer after presenting a surface
	swapChainDesc.Windowed = TRUE;												// output is in windowed mode.

	UINT createDeviceFlags = 0;
#if AGN_DEBUG
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
	if (FAILED(hr))
	{
		logHResultData(hr);

		g_log.debug("E_INVALIDARG when creating Device and Swap chain, Lets try that again with a lower API");

		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_d3d11SwapChain, &m_d3d11Device, &featureLevel,
			&m_d3d11DeviceContext);

		// failed again?
		if (FAILED(hr))
		{
			g_log.error("Failed to create D3D11 Device And Swap Chain");
			logHResultData(hr);

			assert(false);
		}
		else
		{
			g_log.info("Created DX11_0 context");
		}
	}
	else
	{
		g_log.info("Created DX11_1 context");
	}
	
}


glm::ivec2 AGN::DeviceDX11::getMaxMSAASampleQuality()
{
	std::vector<ivec2> levels;
	levels.push_back(ivec2(1, 0)); // NO MSAA

	for (UINT sampleCount = 1; sampleCount <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; sampleCount++)
	{
		UINT maxQualityLevel;
		HRESULT hr = m_d3d11Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &maxQualityLevel);

		if (hr != S_OK)
		{
			g_log.debug("CheckMultisampleQualityLevels failure");
			assert(false);
		}

		if (maxQualityLevel > 0)
		{
			g_log.info("MSAA %u supported with %u quality levels", sampleCount, maxQualityLevel);
			levels.push_back(ivec2(sampleCount, maxQualityLevel-1));
		}
		//else
		//{
			//g_log.debug("MSAA %u NOT supported because it has %u quality levels", sampleCount, maxQualityLevel);
		//}
		
	}


	return levels[levels.size()-1];
}

void AGN::DeviceDX11::onWindowUpdated(glm::ivec2 a_dimensions)
{
	// TODO: investigate if we have to rebuild the whole device including everything!
}

// reference: http://www.rastertek.com/dx11tut03.html
DXGI_RATIONAL AGN::DeviceDX11::queryRefreshRate(bool a_vsync)
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
			if (displayModeList[i].Width == m_window->getDimensions().x && displayModeList[i].Height == m_window->getDimensions().y)
			{
				refreshRate = displayModeList[i].RefreshRate;
			}
		}

		delete[] displayModeList;

		safeRelease(adapterOutput);
		safeRelease(adapter);
		safeRelease(factory);
	}

	return refreshRate;
}

AGN::IMesh* AGN::DeviceDX11::createMesh(const uint16_t a_aId, AGN::MeshData* a_meshData)
{
	ID3D11Buffer* d3d11VertexBuffer = nullptr;
	ID3D11Buffer* d3d11IndexBuffer = nullptr;

	const unsigned int numVertices = static_cast<unsigned int>(a_meshData->positions.size());

	// Create vertex buff
	D3D11_BUFFER_DESC vertexPosBufferDesc;
	memset(&vertexPosBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexPosBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // how the buffer will be bound to the pipeline 
	vertexPosBufferDesc.ByteWidth = static_cast<uint32_t>(sizeof(MeshDX11::VertexShaderData) * numVertices);
	vertexPosBufferDesc.CPUAccessFlags = 0;
	vertexPosBufferDesc.Usage = D3D11_USAGE_DEFAULT; // Identify how the buffer is expected to be read from and written to. Frequency of update is a key factor

	D3D11_SUBRESOURCE_DATA resourceDataVertexBuffer;
	memset(&resourceDataVertexBuffer, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	MeshDX11::VertexShaderData* vsDataToUpload = new MeshDX11::VertexShaderData[numVertices];

	for (uint32_t i = 0; i < numVertices; i++)
	{
		vsDataToUpload[i].position = a_meshData->positions[i];
		vsDataToUpload[i].normal = a_meshData->normals[i];
		vsDataToUpload[i].textureCoords = a_meshData->textureCoords[i];
		vsDataToUpload[i].tangent = a_meshData->tangents[i];
		vsDataToUpload[i].biTangent = a_meshData->bitangents[i];
	}
	
	resourceDataVertexBuffer.pSysMem = vsDataToUpload;

	HRESULT hr = m_d3d11Device->CreateBuffer(&vertexPosBufferDesc, &resourceDataVertexBuffer, &d3d11VertexBuffer);

	delete[] vsDataToUpload;

	if (FAILED(hr))
	{
		g_log.error("failure CreateBuffer vertexBufferDesc");
		return nullptr;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	D3D11_SUBRESOURCE_DATA resourceDataIndexBuffer;
	memset(&resourceDataIndexBuffer, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	indexBufferDesc.ByteWidth = static_cast<uint32_t>(sizeof(uint32_t) * a_meshData->indicies.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	resourceDataIndexBuffer.pSysMem = a_meshData->indicies.data();

	hr = m_d3d11Device->CreateBuffer(&indexBufferDesc, &resourceDataIndexBuffer, &d3d11IndexBuffer);
	if (FAILED(hr))
	{
		g_log.error("failure CreateBuffer indexBufferDesc");
		return nullptr;
	}

	// apply debug names
	D3D11_SET_DEBUG_NAME(d3d11VertexBuffer, "Mesh Vertex buffer (" + a_meshData->relativePath + ")");
	D3D11_SET_DEBUG_NAME(d3d11IndexBuffer, "Mesh Index buffer (" + a_meshData->relativePath + ")");

	// create agnostik texture holding the info.
	MeshDX11* mesh = new MeshDX11(a_aId, a_meshData, d3d11VertexBuffer, d3d11IndexBuffer);
	
	return dynamic_cast<IMesh*>(mesh);
}

AGN::ITexture* AGN::DeviceDX11::createTexture(const uint16_t a_aId, AGN::TextureData* a_textureData)
{
	ID3D11Texture2D* textureHandle = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	const char* relativePath = a_textureData->relativePath.c_str();

	D3D11_TEXTURE2D_DESC textureDesc;
	memset(&textureDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = a_textureData->width;
	textureDesc.Height = a_textureData->height;
	//textureDesc.MipLevels = 1;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// DXGI_FORMAT_R8G8B8A8_UNORM DXGI_FORMAT_R8G8B8A8_TYPELESS DXGI_FORMAT_R8G8B8A8_UINT
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT; //  D3D11_USAGE_DYNAMIC
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	std::vector<D3D11_SUBRESOURCE_DATA> resourceDataList;
	// add data for the first mipmap (level 0)
	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	resourceData.pSysMem = a_textureData->buffer;
	resourceData.SysMemPitch = a_textureData->width * 4;									// pitch in bytes
	resourceData.SysMemSlicePitch = a_textureData->height * resourceData.SysMemPitch;	// pitch * slice (total memsize)
	resourceDataList.push_back(resourceData);

	// generate mipmaps with stb_image
	int currentWidth = a_textureData->width;
	int currentHeight = a_textureData->height;
	int prevWidth = 0;
	int preHeight = 0;
	do
	{
		D3D11_SUBRESOURCE_DATA resourceData;
		memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

		// resize to new level
		prevWidth = currentWidth;
		preHeight = currentHeight;
		currentWidth = max(currentWidth / 2, 1);
		currentHeight = max(currentHeight / 2, 1);
		
		// allocate memory for new mipmap
		resourceData.pSysMem = new uint8_t[4 * currentWidth * currentHeight];
		resourceData.SysMemPitch = currentWidth * 4;									// pitch in bytes
		resourceData.SysMemSlicePitch = currentHeight * resourceData.SysMemPitch;	// pitch * slice (total memsize)

		// resize
		unsigned char* prevBuffer = (unsigned char*)resourceDataList[resourceDataList.size() - 1].pSysMem;
		int success = stbir_resize_uint8(prevBuffer, prevWidth, preHeight, 0, (unsigned char*)resourceData.pSysMem, currentWidth, currentHeight, 0, 4);

		if (!success)
		{
			g_log.error("Failed to resize mipmap");
			assert(success);
		}
		
		resourceDataList.push_back(resourceData);

	} while ((currentWidth > 1) || (currentHeight > 1));

	HRESULT hr = m_d3d11Device->CreateTexture2D(&textureDesc, &resourceDataList[0], &textureHandle);

	if (FAILED(hr))
	{
		g_log.error("Failed to create Texture: %s", relativePath);
		assert(false);
		return nullptr;
	}

	hr = textureHandle->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(relativePath) - 1, relativePath);

	if (FAILED(hr))
	{
		g_log.error("Failed to set private data: %s", relativePath);
		return nullptr;
	}

	// create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset(&SRVDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;

	hr = m_d3d11Device->CreateShaderResourceView(textureHandle, &SRVDesc, &shaderResourceView);

	if (FAILED(hr))
	{
		g_log.error("Failed to create shader resource view: %s", relativePath);
		return nullptr;
	}

	// give debug names 
	D3D11_SET_DEBUG_NAME(textureHandle, "Texture handle (" + a_textureData->relativePath + ")");
	D3D11_SET_DEBUG_NAME(shaderResourceView, "Shader Resource View (" + a_textureData->relativePath + ")");

	// clean mipmap data!
	for (uint32_t i = 1; i < resourceDataList.size(); i++)
	{
		delete[] resourceDataList[i].pSysMem;
	}

	// create agnostik texture holding the data
	TextureDX11* texture = new TextureDX11(a_aId, a_textureData, textureHandle, shaderResourceView);

	return dynamic_cast<ITexture*>(texture);
}

AGN::IShader* AGN::DeviceDX11::createShader(const uint16_t a_aId, const char* a_shaderSource, AGN::EShaderType a_type)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// use latest profile
	std::string profile = ShaderDX11::getLatestProfile(a_type, m_d3d11Device);
	
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if AGN_DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif
	HRESULT hr;

	hr = D3DCompile(
		a_shaderSource,							// A pointer to uncompiled shader data; either ASCII HLSL code or a compiled effect.
		strlen(a_shaderSource),					// Length of pSrcData.
		nullptr,								// Optional.You can use this parameter for strings that specify error messages.
		nullptr,								// Optional. An array of NULL-terminated macro definitions (see D3D_SHADER_MACRO).
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		// Optional. A pointer to an ID3DInclude for handling include files
		"Main",									// The name of the shader entry point function where shader execution begins.
		profile.c_str(),						// specifies the shader target or set of shader features to compile against
		flags,									// Shader compile options.
		0,										// Effect compile options. 
		&shaderBlob,							// interface that you can use to access the compiled code.
		&errorBlob);							// interface that you can use to access compiler error messages, or NULL if there are no errors.

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			g_log.error("Error during compilation of HLSL Shader...");

			std::string errorMessage = (char*)errorBlob->GetBufferPointer();
			g_log.error(errorMessage.c_str());

			safeRelease(shaderBlob);
			safeRelease(errorBlob);

			assert(false);
		}

		return nullptr;
	}

	ID3D11DeviceChild* d3d11Shader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;

	switch (a_type)
	{
	case EShaderType::PixelShader:
		hr = m_d3d11Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader);
		d3d11Shader = dynamic_cast<ID3D11DeviceChild*>(pixelShader);
		D3D11_SET_DEBUG_NAME(d3d11Shader, "Pixel Shader a_ID(" + std::to_string(a_aId) + ")");
		break;

	case EShaderType::VertexShader:
		hr = m_d3d11Device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader);
		d3d11Shader = dynamic_cast<ID3D11DeviceChild*>(vertexShader);
		D3D11_SET_DEBUG_NAME(d3d11Shader, "Vertex Shader a_ID(" + std::to_string(a_aId) + ")");
		break;

	default:
		g_log.error("Unsupported Shader type");
		return nullptr;
	}
	
	if (FAILED(hr))
	{
		g_log.error("Failed to compile shader");
		return nullptr;
	}

	safeRelease(errorBlob);

	ShaderDX11* shader = new ShaderDX11(*this, a_aId, a_type, d3d11Shader, shaderBlob);
	return dynamic_cast<IShader*>(shader);
}

AGN::IShaderPipeline* AGN::DeviceDX11::createShaderPipeline(const uint16_t a_aId, std::vector<AGN::IShader*> a_shaders)
{
	HRESULT hr;

	ShaderPipelineData* shaderPipelineData = new ShaderPipelineData();
	shaderPipelineData->aId = a_aId;

	for (unsigned int i = 0; i < a_shaders.size(); i++)
	{
		ShaderDX11* shaderDX11 = dynamic_cast<ShaderDX11*>(a_shaders[i]);

		switch (shaderDX11->getType())
		{
		case EShaderType::VertexShader:
			shaderPipelineData->vertexShader = a_shaders[i];
			break;

		case EShaderType::PixelShader:
			shaderPipelineData->pixelShader = a_shaders[i];
			break;

		case EShaderType::HullShader:
			shaderPipelineData->hullShader = a_shaders[i];
			break;

		case EShaderType::DomainShader:
			shaderPipelineData->domainShader = a_shaders[i];
			break;

		case EShaderType::GeometryShader:
			shaderPipelineData->geometryShader = a_shaders[i];
			break;

		case EShaderType::ComputeShader:
			shaderPipelineData->computeShader = a_shaders[i];
			break;
		}
	}

	if (shaderPipelineData->vertexShader == nullptr || shaderPipelineData->pixelShader == nullptr)
	{
		g_log.error("Shader pipeline is missing either a vertex shader or a pixel shader?");
		return nullptr;
	}

	ShaderDX11* vertexShaderDX11 = dynamic_cast<ShaderDX11*>(shaderPipelineData->vertexShader);

	// create input layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = nullptr;
	int inputLayoutCount = 0;

	vertexShaderDX11->getInputLayoutDesc(inputLayoutDesc, inputLayoutCount);

	ID3D11InputLayout* vertexInputLayout = nullptr;
	hr = m_d3d11Device->CreateInputLayout(
		inputLayoutDesc,												// input-assembler stage input data types
		inputLayoutCount,
		vertexShaderDX11->getBlob()->GetBufferPointer(),				// pointer to the compiled shader
		vertexShaderDX11->getBlob()->GetBufferSize(),
		&vertexInputLayout);

	delete[] inputLayoutDesc;

	if (FAILED(hr))
	{
		g_log.error("Failure CreateInputLayout");
		return nullptr;
	}

	// create sampler layout for pixel shader
	ShaderDX11* pixelShaderDX11 = dynamic_cast<ShaderDX11*>(shaderPipelineData->pixelShader);
	ID3D11SamplerState* samplerState = nullptr;
	D3D11_SAMPLER_DESC* samplerLayoutDesc = nullptr;
	int samplerLayoutCount = 0;
	pixelShaderDX11->getSamplerLayoutDesc(samplerLayoutDesc, samplerLayoutCount);

	// TODO: support more? if not, refactor this code
	if (samplerLayoutCount > 1)
	{
		g_log.warning("More than 1 sampler is not yet supported");
	}

	for (int i = 0; i < 1 /*samplerLayoutCount*/; i++)
	{
		// Create the texture sampler state.
		hr = m_d3d11Device->CreateSamplerState(&samplerLayoutDesc[i], &samplerState);
		if (FAILED(hr))
		{
			g_log.error("Failed to initialize sampler state");
			return nullptr;
		}
	}

	delete[] samplerLayoutDesc;

	// add debug labels
	D3D11_SET_DEBUG_NAME(vertexInputLayout, "Shader Vertex input layout(" + std::to_string(a_aId) + ")");
	D3D11_SET_DEBUG_NAME(samplerState, "Shader sampler state(" + std::to_string(a_aId) + ")");

	ShaderPipelineDX11* shaderPipeline = new ShaderPipelineDX11(shaderPipelineData, vertexInputLayout, samplerState);

	// cleanup objects
	delete shaderPipelineData;

	return dynamic_cast<IShaderPipeline*>(shaderPipeline);
}

void AGN::DeviceDX11::setDebugName(ID3D11DeviceChild* a_child, const std::string& a_name, const uint32_t a_lineNum, const char* a_fileName)
{
#ifdef AGN_DEBUG
	if (a_child != nullptr && a_name.length() > 0)
	{
		std::string label = a_name;
		if (a_lineNum != 0 && strlen(a_fileName) > 0)
		{
			label += " - LineNum: " + std::to_string(a_lineNum) + " - File: " + std::string(a_fileName);
		}

		a_child->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)label.size(), label.c_str());
	}
#endif
}

void AGN::DeviceDX11::logLiveObjects()
{
#ifdef AGN_DEBUG
	if (m_d3dDebug)	m_d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif // AGN_DEBUG
}

// TODO: Document
// Reference: http://seanmiddleditch.com/direct3d-11-debug-api-tricks/
void AGN::DeviceDX11::beginDebugEvent(const std::string& a_eventName)
{
#ifdef AGN_DEBUG
	if (m_debugUDA)
	{
		// annoyingly convert to WideChar (Thanks Microsoft!)
		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, a_eventName.c_str(), -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, a_eventName.c_str(), -1, wstr, wchars_num);
		
		// actually start the actual event
		m_debugUDA->BeginEvent(wstr);
		delete[] wstr;
	}
#endif
}

// TODO: Document
// Reference: http://seanmiddleditch.com/direct3d-11-debug-api-tricks/
void AGN::DeviceDX11::setDebugMarker(const std::string& a_markerName)
{
#ifdef AGN_DEBUG
	if (m_debugUDA)
	{
		// annoyingly convert to WideChar (Thanks Microsoft!)
		int wchars_num = MultiByteToWideChar(CP_UTF8, 0, a_markerName.c_str(), -1, NULL, 0);
		wchar_t* wstr = new wchar_t[wchars_num];
		MultiByteToWideChar(CP_UTF8, 0, a_markerName.c_str(), -1, wstr, wchars_num);

		// set the debug marker
		m_debugUDA->SetMarker(wstr);
		
		delete[] wstr;
	}
#endif
}

// TODO: Document
// Reference: http://seanmiddleditch.com/direct3d-11-debug-api-tricks/
void AGN::DeviceDX11::endDebugEvent()
{
#ifdef AGN_DEBUG
	if (m_debugUDA) m_debugUDA->EndEvent();
#endif
}

