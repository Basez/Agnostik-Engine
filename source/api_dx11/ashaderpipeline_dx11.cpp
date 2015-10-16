#include "asharedh.hpp"
#include "ashaderpipeline_dx11.hpp"
#include "ashader_dx11.hpp"
#include "aosutils.hpp"
#include "adevice_dx11.hpp"

#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace glm;

AGN::AShaderPipelineDX11::AShaderPipelineDX11(ADeviceDX11& a_deviceReference, AShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState)
	: m_deviceReference(a_deviceReference)
	, m_shaderPipelineData(a_shaderPipelineData)
	, m_vertexInputLayout(a_vertexInputLayout)
	, m_samplerState(a_samplerState)
{
	AShaderDX11* vertexShader = dynamic_cast<AShaderDX11*>(m_shaderPipelineData->vertexShader);

	IAShader * const shaderArray[] = {
		m_shaderPipelineData->vertexShader,
		m_shaderPipelineData->pixelShader,
		m_shaderPipelineData->hullShader,
		m_shaderPipelineData->domainShader,
		m_shaderPipelineData->geometryShader,
		m_shaderPipelineData->computeShader
	};

	const uint32_t numShaders = sizeof(shaderArray) / sizeof(IAShader*);

	// create constant buffers for shader pipeline, combined from all shaders (same as OpenGL)
	for (unsigned int i = 0; i < numShaders; i++)
	{
		if (shaderArray[i] == nullptr) continue;

		AShaderDX11* currentShader = dynamic_cast<AShaderDX11*>(shaderArray[i]);

		// create input layout for vertex shader
		D3D11_SHADER_BUFFER_DESC* constReflectionBufferDesc = nullptr;
		int inputLayoutCount = 0;

		currentShader->getConstantBufferDesc(constReflectionBufferDesc, inputLayoutCount);

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

			HRESULT hr = m_deviceReference.getD3D11Device()->CreateBuffer(&newConstantBufferDesc, nullptr, &constantBufferDX11->bufferHandle);

			if (FAILED(hr))
			{
				g_log.error("failure creating constant buffer");
				return;
			}

			m_constantBuffers.push_back(constantBufferDX11);
		}

		delete[] constReflectionBufferDesc;
	}

	/*
	// update object constant buffer with current position
	g_engine.getD3DDeviceContext()->UpdateSubresource(m_d3dConstantBuffers[CB_Object], 0, nullptr, &g_worldMatrix, 0, 0);
	*/
}

void AGN::AShaderPipelineDX11::bind()
{
	// TODO:
}

void AGN::AShaderPipelineDX11::setConstantBufferData(const char* a_name, void* a_data, size_t a_dataSize)
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

bool AGN::AShaderPipelineDX11::hasConstantBuffer(const char* a_name)
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