#pragma once

#include "iashaderpipeline.hpp"

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineGL : public IAShaderPipeline
	{
	public:
		AShaderPipelineGL(const GLuint a_glprogramId, AShaderPipelineData& a_data);
		class IAShader* getShader(const EAShaderType a_type) override;
		uint16_t getAId() { return m_aId; }

		void bind() override;
		void setConstantBufferData(const EAShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasConstantBuffer(const EAShaderType a_shader, const char* a_name) override;

		GLuint getGlProgramId() { return m_glProgramId; }
		GLint getUniformIdByName(const char* a_name);
		bool hasUniform(const char* a_name);
		struct AUniformConstantBufferGL* getUniformConstantBufferByName(const char* a_name);

	private:

		const uint16_t m_aId;				
		const GLuint m_glProgramId;			
		class IAShader* m_shaders[6];

		std::vector<struct AUniformConstantBufferGL*> m_constantBuffers;

		GLint m_uniformPropertyCount;
	};	

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
	};
}