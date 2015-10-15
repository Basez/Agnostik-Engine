#include "asharedh.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "ashader_dx11.hpp"
#include "aosutils.hpp"
#include "adevice_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace glm;

AGN::AShaderPipelineDX11::AShaderPipelineDX11(ADeviceDX11& a_deviceReference, AShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState)
	: m_shaderPipelineData(a_shaderPipelineData)
	, m_vertexInputLayout(a_vertexInputLayout)
	, m_samplerState(a_samplerState)
{

	AShaderDX11* vertexShader = dynamic_cast<AShaderDX11*>(m_shaderPipelineData->vertexShader);

	int numVertexConstantBuffers = 0;
	D3D11_SHADER_BUFFER_DESC* shaderBufferDecs;
	vertexShader->getConstantBufferDesc(shaderBufferDecs, numVertexConstantBuffers); // TODO:

	// TODO: finish this

	// Create the constant buffers for the variables defined in the vertex shader.
	/*
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(mat4);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_d3d11Device->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dConstantBuffers[CB_Appliation]);
	if (FAILED(hr))
	{
	Log.error("failure CreateBuffer");
	return;
	}
	hr = d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dConstantBuffers[CB_Frame]);
	if (FAILED(hr))
	{
	Log.error("failure CreateBuffer");
	return;
	}
	hr = d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dConstantBuffers[CB_Object]);
	if (FAILED(hr))
	{
	Log.error("failure CreateBuffer");
	return;
	}

	// update object constant buffer with current position
	g_engine.getD3DDeviceContext()->UpdateSubresource(m_d3dConstantBuffers[CB_Object], 0, nullptr, &g_worldMatrix, 0, 0);

	*/

}

void AGN::AShaderPipelineDX11::bind()
{
	// TODO:
}


void AGN::AShaderPipelineDX11::setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize)
{
	// TODO:
}

bool AGN::AShaderPipelineDX11::hasUniformBuffer(const char* a_name)
{
	return false; // TODO:
}