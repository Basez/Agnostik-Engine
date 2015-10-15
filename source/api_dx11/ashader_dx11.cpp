#include "asharedh.hpp"
#include "ashader_dx11.hpp"
#include "aerrorutils_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>

AGN::AShaderDX11::AShaderDX11(const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle, ID3DBlob* a_shaderBlob)
	: m_aId(a_aId)
	, m_type(a_type)
	, m_shaderHandle(a_shaderHandle)
	, m_shaderBlob(a_shaderBlob)
{
	D3D11_SHADER_DESC shaderDesc;

	HRESULT hr = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_shaderReflection);
	
	hr = m_shaderReflection->GetDesc(&shaderDesc);

#ifdef AGN_DEBUG
	//AGN::logHResultData(hr);
#endif // AGN_DEBUG

	/*
	// get Input data names
	{
		D3D11_SIGNATURE_PARAMETER_DESC	paramDesc;

		for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			m_shaderReflection->GetInputParameterDesc(i, &paramDesc);
			
			g_log.debug("-- Found Input parameter:");
			g_log.debug("SemanticName: %s", paramDesc.SemanticName);
			g_log.debug("Register: %u", paramDesc.Register);
			g_log.debug("ComponentType: %u", (uint32_t)paramDesc.ComponentType);
		}
	}
	*/
	// get constant buffers
	{
		for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++)
		{
			D3D11_SHADER_BUFFER_DESC constantBufferDesc;

			ID3D11ShaderReflectionConstantBuffer* buffer = m_shaderReflection->GetConstantBufferByIndex(i);

			hr = buffer->GetDesc(&constantBufferDesc);

			g_log.debug("-- Found Constant Buffer:");
			g_log.debug("Name: %s", constantBufferDesc.Name);
			g_log.debug("Size: %u", (uint32_t)constantBufferDesc.Size);
			g_log.debug("Variables: %u", (uint32_t)constantBufferDesc.Variables);
			g_log.debug("Type: %u", (uint32_t)constantBufferDesc.Type);
		}
	}

	g_log.debug("end of shader reflection");
	
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
	D3D11_SHADER_DESC shaderDesc;

	m_shaderReflection->GetDesc(&shaderDesc);

	out_count = shaderDesc.InputParameters;
	out_inputLayouts = new D3D11_INPUT_ELEMENT_DESC[out_count];
	ZeroMemory(out_inputLayouts, sizeof(D3D11_INPUT_ELEMENT_DESC) * out_count);

	// get Input data names
	{
		for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
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
}