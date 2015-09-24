#pragma once

#include "iashaderpipeline.hpp"

namespace AGN
{
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

	private:
		const uint16_t m_aId;				
		const GLuint m_glProgramId;			
		class IAShader* m_vertextShader;	
		class IAShader* m_pixelShader;		
		class IAShader* m_hullShader;		
		class IAShader* m_domainShader;		
		class IAShader* m_geometryShader;	
		class IAShader* m_computeShader;

		GLint m_uniformCount;
		std::string* m_uniformNames;
		GLint* m_uniformIDs;

	};	
}