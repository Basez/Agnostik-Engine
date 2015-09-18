#include "asharedh.hpp"
#include "ashaderpipeline_gl.hpp"
#include "ashader_gl.hpp"

AGN::AShaderPipelineGL::AShaderPipelineGL(const GLuint a_glprogramId, AShaderPipelineData& a_data)
	: m_aId(a_data.aId)
	, m_glProgramId(a_glprogramId)
	, m_vertextShader(a_data.vertextShader)
	, m_pixelShader(a_data.pixelShader)
	, m_hullShader(a_data.hullShader)
	, m_domainShader(a_data.domainShader)
	, m_geometryShader(a_data.geometryShader)
	, m_computeShader(a_data.computeShader)
{

}

/*
GLint AGN::AMaterialGL::getUniformLocation(const char* a_title)
{
	GLint uniformID = glGetUniformLocation(m_programID, a_title);

	if (uniformID == -1)
	{
		g_log.error("Uniform: %s == %i", a_title, uniformID);
	}

	return uniformID;
}

GLint AGN::AMaterialGL::getAttribLocation(const char* a_title)
{
	GLint attribID = glGetAttribLocation(m_programID, a_title);

	if (attribID == -1)
	{
		g_log.error("Attrib: %s == %i", a_title, attribID);
	}

	return attribID;
}
*/