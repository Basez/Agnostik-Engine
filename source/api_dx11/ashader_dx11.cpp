#include "asharedh.hpp"
#include "ashader_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>


AGN::AShaderDX11::AShaderDX11(const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle)
	: m_aId(a_aId)
	, m_type(a_type)
	, m_shaderHandle(a_shaderHandle)
{

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
