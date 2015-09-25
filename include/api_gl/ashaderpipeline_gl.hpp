#pragma once

#include "iashaderpipeline.hpp"

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineGL : public IAShaderPipeline
	{
	public:
		AShaderPipelineGL(const GLuint a_glprogramId, AShaderPipelineData& a_data);
		class IAShader* getVertextShader() override { return m_vertextShader; }
		class IAShader* getPixelShader() override { return m_pixelShader; }
		class IAShader* getHullShader() override { return m_hullShader; }
		class IAShader* getDomainShader() override { return m_domainShader; }
		class IAShader* getGeometryShader() override { return m_geometryShader; }
		class IAShader* getComputeShader() override { return m_computeShader; }
		uint16_t getAId() { return m_aId; }

		void bind() override;

		// TODO: change architecture after bind stuff works
		GLuint getGlProgramId() { return m_glProgramId; }
		GLint getUniformIdByName(const char* a_name);
		void setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize);
		struct AUniformConstantBufferGL* getUniformConstantBufferByName(const char* a_name);

	private:
		const uint16_t m_aId;				
		const GLuint m_glProgramId;			
		class IAShader* m_vertextShader;	
		class IAShader* m_pixelShader;		
		class IAShader* m_hullShader;		
		class IAShader* m_domainShader;		
		class IAShader* m_geometryShader;	
		class IAShader* m_computeShader;

		std::vector<struct AUniformConstantBufferGL*> m_uniformBuffers;

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