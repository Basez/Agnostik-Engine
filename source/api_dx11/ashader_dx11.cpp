#include "asharedh.hpp"
#include "ashader_dx11.hpp"
#include "aerrorutils_dx11.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "adevice_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>

AGN::AShaderDX11::AShaderDX11(ADeviceDX11& a_deviceReference, const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle, ID3DBlob* a_shaderBlob)
	: m_deviceReference(a_deviceReference)
	, m_aId(a_aId)
	, m_type(a_type)
	, m_shaderHandle(a_shaderHandle)
	, m_shaderBlob(a_shaderBlob)
	, m_shaderReflection(nullptr)
	, m_shaderReflectionDesc(nullptr)
{
	HRESULT hr = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_shaderReflection);

	if (FAILED(hr)) g_log.error("Failed performing reflection on shader");

	m_shaderReflectionDesc = new D3D11_SHADER_DESC();
	ZeroMemory(m_shaderReflectionDesc, sizeof(D3D11_SHADER_DESC));
	hr = m_shaderReflection->GetDesc(m_shaderReflectionDesc);

	if (FAILED(hr)) g_log.error("Failed getting reflection desc on shader");

	D3D11_SHADER_BUFFER_DESC* constReflectionBufferDesc = nullptr;
	int inputLayoutCount = 0;

	getConstantBufferDesc(constReflectionBufferDesc, inputLayoutCount);

	// create constant buffers
	for (uint16_t i = 0; i < inputLayoutCount; i++)
	{
		// create dx11 constant buffer object and put info in
		AConstantBufferDX11* constantBufferDX11 = new AConstantBufferDX11();
		constantBufferDX11->bufferDesc = new D3D11_SHADER_BUFFER_DESC();
		memcpy(constantBufferDX11->bufferDesc, &constReflectionBufferDesc[i], sizeof(D3D11_SHADER_BUFFER_DESC));

		// create handle to actual buffer
		D3D11_BUFFER_DESC newConstantBufferDesc;
		ZeroMemory(&newConstantBufferDesc, sizeof(D3D11_BUFFER_DESC));

		newConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		newConstantBufferDesc.ByteWidth = constantBufferDX11->bufferDesc->Size;
		newConstantBufferDesc.CPUAccessFlags = 0;
		newConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		newConstantBufferDesc.MiscFlags = 0;
		newConstantBufferDesc.StructureByteStride = 0;

		// TODO: think if this is the correct location for this function, in essence, we are doing the devices job in this constructor?
		HRESULT hr = a_deviceReference.getD3D11Device()->CreateBuffer(&newConstantBufferDesc, nullptr, &constantBufferDX11->bufferHandle);

		if (FAILED(hr))
		{
			g_log.error("failure creating constant buffer");
			return;
		}

		m_constantBuffers.push_back(constantBufferDX11);
	}

	delete[] constReflectionBufferDesc;


#ifdef AGN_DEBUG
	//AGN::logHResultData(hr);
#endif // AGN_DEBUG
	
}

std::string AGN::AShaderDX11::getLatestProfile(const AGN::EAShaderType a_type, ID3D11Device* a_device)
{
	assert(a_device);

	std::string typePrefix;

	switch (a_type)
	{
	case EAShaderType::VertexShader:
		typePrefix = "vs_";
		break;
	case EAShaderType::PixelShader:
		typePrefix = "ps_";
		break;
	default:
		g_log.error("Unsupported Shader Type");
		break;
	}

	// Query the current feature level:
	D3D_FEATURE_LEVEL featureLevel = a_device->GetFeatureLevel();
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
	case D3D_FEATURE_LEVEL_11_0:
		return typePrefix + "5_0";

	case D3D_FEATURE_LEVEL_10_1:
		return typePrefix + "4_1";

	case D3D_FEATURE_LEVEL_10_0:
		return typePrefix + "4_0";

	case D3D_FEATURE_LEVEL_9_3:
		return typePrefix + "4_0_level_9_3";

	case D3D_FEATURE_LEVEL_9_2:
	case D3D_FEATURE_LEVEL_9_1:
		return typePrefix + "4_0_level_9_1";

	}

	g_log.error("no Profile found for shaderType?");

	return "";
}

void AGN::AShaderDX11::getInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC*& out_inputLayouts, int& out_count)
{
	out_count = m_shaderReflectionDesc->InputParameters;
	out_inputLayouts = new D3D11_INPUT_ELEMENT_DESC[out_count];
	ZeroMemory(out_inputLayouts, sizeof(D3D11_INPUT_ELEMENT_DESC) * out_count);

	// get Input data names
	for (uint32_t i = 0; i < m_shaderReflectionDesc->InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC	paramDesc;

		m_shaderReflection->GetInputParameterDesc(i, &paramDesc);

		g_log.debug("-- Found Input parameter:");
		g_log.debug("SemanticName: %s", paramDesc.SemanticName);
		g_log.debug("Register: %u", paramDesc.Register);
		g_log.debug("ComponentType: %u", (uint32_t)paramDesc.ComponentType);

		//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		out_inputLayouts[i].SemanticName = paramDesc.SemanticName;
		out_inputLayouts[i].SemanticIndex = paramDesc.SemanticIndex;
		out_inputLayouts[i].Format = (DXGI_FORMAT)getFormatByParameterSignature(paramDesc);
		out_inputLayouts[i].InputSlot = 0;	
		out_inputLayouts[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		out_inputLayouts[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		out_inputLayouts[i].InstanceDataStepRate = 0;
	}
}

uint32_t AGN::AShaderDX11::getFormatByParameterSignature(D3D11_SIGNATURE_PARAMETER_DESC& a_signature)
{
	// determine DXGI format
	if (a_signature.Mask == 1)
	{
		if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return (uint32_t)DXGI_FORMAT_R32_UINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return (uint32_t)DXGI_FORMAT_R32_SINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return (uint32_t)DXGI_FORMAT_R32_FLOAT;
	}
	else if (a_signature.Mask <= 3)
	{
		if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return (uint32_t)DXGI_FORMAT_R32G32_UINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return (uint32_t)DXGI_FORMAT_R32G32_SINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return (uint32_t)DXGI_FORMAT_R32G32_FLOAT;
	}
	else if (a_signature.Mask <= 7)
	{
		if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return (uint32_t)DXGI_FORMAT_R32G32B32_UINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return (uint32_t)DXGI_FORMAT_R32G32B32_SINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return (uint32_t)DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (a_signature.Mask <= 15)
	{
		if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return (uint32_t)DXGI_FORMAT_R32G32B32A32_UINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return (uint32_t)DXGI_FORMAT_R32G32B32A32_SINT;
		else if (a_signature.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return (uint32_t)DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	g_log.error("Unexpected signature Mask or Component Type found in shader");
	return -1;
}

void AGN::AShaderDX11::getSamplerLayoutDesc(D3D11_SAMPLER_DESC*& out_samplerLayoutDecs, int& out_count)
{
	D3D11_SHADER_INPUT_BIND_DESC paramDesc;

	for (uint32_t i = 0; i < m_shaderReflectionDesc->BoundResources; i++)
	{
		m_shaderReflection->GetResourceBindingDesc(i, &paramDesc);

		//g_log.debug("-- Found Input parameter:");
		//g_log.debug("SemanticName: %s", paramDesc.Name);
		//g_log.debug("Register: %u", paramDesc.Register);
		//g_log.debug("ComponentType: %u", (uint32_t)paramDesc.ComponentType);

		if (paramDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER)
		{
			out_count = 1; // Hardcoded to 1, only 1 sampler is supported at the moment
			out_samplerLayoutDecs = new D3D11_SAMPLER_DESC[out_count];
			ZeroMemory(out_samplerLayoutDecs, sizeof(D3D11_SAMPLER_DESC) * out_count);

			out_samplerLayoutDecs[0].Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			out_samplerLayoutDecs[0].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			out_samplerLayoutDecs[0].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			out_samplerLayoutDecs[0].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			out_samplerLayoutDecs[0].MipLODBias = 0.0f;
			out_samplerLayoutDecs[0].MaxAnisotropy = 1;
			out_samplerLayoutDecs[0].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			out_samplerLayoutDecs[0].BorderColor[0] = 0;
			out_samplerLayoutDecs[0].BorderColor[1] = 0;
			out_samplerLayoutDecs[0].BorderColor[2] = 0;
			out_samplerLayoutDecs[0].BorderColor[3] = 0;
			out_samplerLayoutDecs[0].MinLOD = 0;
			out_samplerLayoutDecs[0].MaxLOD = D3D11_FLOAT32_MAX;

			return;
		}

		/*
		typedef struct _D3D11_SHADER_INPUT_BIND_DESC
		{
			LPCSTR                      Name;           // Name of the resource
			D3D_SHADER_INPUT_TYPE       Type;           // Type of resource (e.g. texture, cbuffer, etc.)
			UINT                        BindPoint;      // Starting bind point
			UINT                        BindCount;      // Number of contiguous bind points (for arrays)

			UINT                        uFlags;         // Input binding flags
			D3D_RESOURCE_RETURN_TYPE    ReturnType;     // Return type (if texture)
			D3D_SRV_DIMENSION           Dimension;      // Dimension (if texture)
			UINT                        NumSamples;     // Number of samples (0 if not MS texture)
		} D3D11_SHADER_INPUT_BIND_DESC;
		*/
	
	}

	// none found?
	out_count = 0;
	out_samplerLayoutDecs = nullptr;
}

void AGN::AShaderDX11::getConstantBufferDesc(D3D11_SHADER_BUFFER_DESC*& out_constantBufferDecs, int& out_count)
{	
	std::vector<D3D11_SHADER_BUFFER_DESC> constantBufferDescList;

	// get constant buffer info
	for (uint32_t i = 0; i < m_shaderReflectionDesc->ConstantBuffers; i++)
	{
		D3D11_SHADER_BUFFER_DESC constantBufferDesc;

		ID3D11ShaderReflectionConstantBuffer* buffer = m_shaderReflection->GetConstantBufferByIndex(i);

		HRESULT hr = buffer->GetDesc(&constantBufferDesc);

		if (constantBufferDesc.Type == D3D_CT_CBUFFER)
		{
			// add to list
			constantBufferDescList.push_back(constantBufferDesc);
		}
	}

	out_count = (int)constantBufferDescList.size();
	out_constantBufferDecs = new D3D11_SHADER_BUFFER_DESC[out_count];

	memcpy(out_constantBufferDecs, constantBufferDescList.data(), sizeof(D3D11_SHADER_BUFFER_DESC) * constantBufferDescList.size());
}

void AGN::AShaderDX11::setConstantBufferData(const char* a_name, void* a_data, size_t a_dataSize)
{
	for (uint16_t i = 0; i < m_constantBuffers.size(); i++)
	{
		if (strcmp(m_constantBuffers[i]->bufferDesc->Name, a_name) == 0)
		{
			m_deviceReference.getD3D11DeviceContext()->UpdateSubresource(m_constantBuffers[i]->bufferHandle, 0, nullptr, a_data, 0, 0);

			return;
		}
	}
}

bool AGN::AShaderDX11::hasConstantBuffer(const char* a_name)
{
	for (uint16_t i = 0; i < m_constantBuffers.size(); i++)
	{
		if (strcmp(m_constantBuffers[i]->bufferDesc->Name, a_name) == 0)
		{
			return true;
		}
	}

	return false;
}
