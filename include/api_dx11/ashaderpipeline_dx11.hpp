#pragma once

#include "iashaderpipeline.hpp"

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineDX11 : public IAShaderPipeline
	{
	public:
		AShaderPipelineDX11();
		class IAShader* getVertexShader() override { return nullptr; } // TODO:
		class IAShader* getPixelShader() override { return nullptr; } // TODO:
		class IAShader* getHullShader() override { return nullptr; } // TODO:
		class IAShader* getDomainShader() override { return nullptr; } // TODO:
		class IAShader* getGeometryShader() override { return nullptr; } // TODO:
		class IAShader* getComputeShader() override { return nullptr; } // TODO:

		void bind() override;
		void setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasUniformBuffer(const char* a_name) override;

	private:

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