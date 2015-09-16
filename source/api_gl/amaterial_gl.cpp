#include "asharedh.hpp"
#include "amaterial_gl.hpp"
#include "ashader_gl.hpp"

AGN::AMaterialGL::AMaterialGL(uint16_t a_id, std::string a_name, GLuint a_programID, std::vector<IAShader*> a_shaders)
	: m_id(a_id)
	, m_name(a_name)
	, m_programID(a_programID)
{
	for (unsigned int i = 0; i < a_shaders.size(); i++)
	{
		m_shaders.push_back(dynamic_cast<AShaderGL*>(a_shaders[i]));
	}
}

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

