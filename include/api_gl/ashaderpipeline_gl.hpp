#pragma once

#include "iashaderpipeline.hpp"

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineGL : public IAShaderPipeline
	{
	public:
		AShaderPipelineGL(const uint32_t a_glprogramId, AShaderPipelineData& a_data);
		class IAShader* getShader(const EAShaderType a_type) override;
		uint16_t getAId() { return m_aId; }

		void bind() override;
		void setConstantBufferData(const EAShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasConstantBuffer(const EAShaderType a_shader, const char* a_name) override;

		uint32_t getGlProgramId() { return m_glProgramId; }
		int32_t getUniformIdByName(const char* a_name);
		bool hasUniform(const char* a_name);
		struct AUniformConstantBufferGL* getUniformConstantBufferByName(const char* a_name);

	private:

		const uint16_t m_aId;				
		const uint32_t m_glProgramId;
		class IAShader* m_shaders[6];

		std::vector<struct AUniformConstantBufferGL*> m_constantBuffers;

		int32_t m_uniformPropertyCount;
	};	

	struct AUniformConstantBufferGL
	{
		int32_t index;
		char name[MAX_UNIFORM_NAME];
		int32_t size;
		uint32_t uboHandle;
		uint8_t* buffer;
		int32_t uniformCount;
		
		// inner uniform members (arrays of their data)
		int32_t* uniformIds;
		int32_t* uniformOffsets;
	};
}