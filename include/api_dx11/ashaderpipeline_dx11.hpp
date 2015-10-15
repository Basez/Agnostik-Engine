#pragma once

#include "iashaderpipeline.hpp"

// fwd declare
struct ID3D11InputLayout;
struct ID3D11SamplerState;

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineDX11 : public IAShaderPipeline
	{
	public:
		AShaderPipelineDX11(class ADeviceDX11& a_deviceReference, AShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState);

		class IAShader* getVertexShader() override { return m_shaderPipelineData->vertexShader; }
		class IAShader* getPixelShader() override { return m_shaderPipelineData->pixelShader; }
		class IAShader* getHullShader() override { return m_shaderPipelineData->hullShader; }
		class IAShader* getDomainShader() override { return m_shaderPipelineData->domainShader; }
		class IAShader* getGeometryShader() override { return m_shaderPipelineData->geometryShader; }
		class IAShader* getComputeShader() override { return m_shaderPipelineData->computeShader; }

		uint16_t getAId() { return m_shaderPipelineData->aId; }
		void bind() override;
		void setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasUniformBuffer(const char* a_name) override;

	private:
		AShaderPipelineData* m_shaderPipelineData;
		ID3D11InputLayout* m_vertexInputLayout;
		ID3D11SamplerState* m_samplerState;

		// TODO:
		//std::vector<struct AUniformConstantBufferGL*> m_uniformBuffers;
	};	

	/*
	// TODO:
	struct AUniformConstantBufferGL
	{
		GLint index;
		GLchar name[MAX_UNIFORM_NAME];
		GLint size;
		GLuint uboHandle;
		GLubyte* buffer;
		GLint uniformCount;
		
		// inner uniform members (arrays of their data)
		GLint* uniformIds;
		GLint* uniformOffsets;
	};*/
}