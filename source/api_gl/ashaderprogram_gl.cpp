#include "asharedh.hpp"
#include "ashaderprogram_gl.hpp"
#include "ashader_gl.hpp"

AGN::AShaderProgramGL::AShaderProgramGL(GLuint a_programID, std::vector<IAShader*> a_shaders)
	: m_programID(a_programID)
{
	for (int i = 0; a_shaders.size(); i++)
	{
		m_shaders.push_back(dynamic_cast<AShaderGL*>(a_shaders[i]));
	}
}

GLint AGN::AShaderProgramGL::getUniformLocation(const char* a_title)
{
	GLint uniformID = glGetUniformLocation(m_programID, a_title);

	if (uniformID == -1)
	{
		g_log.error("Uniform: %s == %i", a_title, uniformID);
	}

	return uniformID;
}

GLint AGN::AShaderProgramGL::getAttribLocation(const char* a_title)
{
	GLint attribID = glGetAttribLocation(m_programID, a_title);

	if (attribID == -1)
	{
		g_log.error("Attrib: %s == %i", a_title, attribID);
	}

	return attribID;
}