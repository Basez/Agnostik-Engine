#include "asharedh.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "ashader_dx11.hpp"
#include "aosutils.hpp"
#include "adevice_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace glm;

AGN::AShaderPipelineDX11::AShaderPipelineDX11(AShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState)
	: m_aId(a_shaderPipelineData->aId)
	, m_vertexInputLayout(a_vertexInputLayout)
	, m_samplerState(a_samplerState)
{
	// fill shader array
	m_shaders[0] = a_shaderPipelineData->vertexShader;
	m_shaders[1] = a_shaderPipelineData->pixelShader;
	m_shaders[2] = a_shaderPipelineData->hullShader;
	m_shaders[3] = a_shaderPipelineData->domainShader;
	m_shaders[4] = a_shaderPipelineData->geometryShader;
	m_shaders[5] = a_shaderPipelineData->computeShader;

	/*
	// update object constant buffer with current position
	g_engine.getD3DDeviceContext()->UpdateSubresource(m_d3dConstantBuffers[CB_Object], 0, nullptr, &g_worldMatrix, 0, 0);
	*/
}

class AGN::IAShader* AGN::AShaderPipelineDX11::getShader(const AGN::EAShaderType a_type)
{
	const uint32_t numShaders = sizeof(m_shaders) / sizeof(m_shaders[0]);

	for (int i = 0; i < numShaders; i++)
	{
		if (m_shaders[i]->getType() == a_type) return m_shaders[i];
	}

	g_log.error("Shader with type not recognized or added");
	assert(false);
	return nullptr;
}

void AGN::AShaderPipelineDX11::bind()
{
	// TODO:? remove this ?
}

void AGN::AShaderPipelineDX11::setConstantBufferData(const EAShaderType a_shaderType, const char* a_name, void* a_data, size_t a_dataSize)
{
	AShaderDX11* shaderDX11 = dynamic_cast<AShaderDX11*>(getShader(a_shaderType));
	shaderDX11->setConstantBufferData(a_name, a_data, a_dataSize);
}

bool AGN::AShaderPipelineDX11::hasConstantBuffer(const EAShaderType a_shader, const char* a_name)
{
	AShaderDX11* shaderDX11 = dynamic_cast<AShaderDX11*>(getShader(a_shader));
	return shaderDX11->hasConstantBuffer(a_name);
}