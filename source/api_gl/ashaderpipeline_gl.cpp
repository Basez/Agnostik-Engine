#include "asharedh.hpp"
#include "ashaderpipeline_gl.hpp"
#include "ashader_gl.hpp"

using namespace glm;

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
	// get total uniform count
	glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORMS, &m_uniformCount);

	//void glGetUniformIndices?(GLuint program?, GLsizei uniformCount?, const char ** uniformNames?, GLuint *uniformIndices?);
	//void glGetActiveUniformName?(GLuint program?, GLuint uniformIndex?, GLsizei bufSize?, GLsizei *length?, char *uniformName?);
	//void glGetActiveUniformsiv?(GLuint program?, GLsizei uniformCount?, const GLuint *uniformIndices?, GLenum pname?, GLint *params?);

	// retrieve all uniform names
	m_uniformNames = new std::string[m_uniformCount];
	m_uniformIDs = new GLint[m_uniformCount];
	for (int i = 0; i < m_uniformCount; i++)
	{
		const GLsizei bufSize = 128;
		GLchar uniformName[bufSize];
		GLsizei length = -1;
		glGetActiveUniformName(m_glProgramId, i, bufSize, &length, uniformName);

		m_uniformNames[i] = std::string(uniformName);
		m_uniformIDs[i] = glGetUniformLocation(m_glProgramId, uniformName);

		GLsizei uniformStringLength = 0;
		int uniformSize = 0;
		GLenum uniformType = 0;
		glGetActiveUniform(m_glProgramId, m_uniformIDs[i], bufSize, &uniformStringLength, &uniformSize, &uniformType, uniformName);
		//GL_FLOAT_VEC4
		//int asda = 1;
	}


	/*
	// get indices of uniform variables in uniform block
	// PS. Check whether all indices != GL_INVALID_INDEX
	//glGetUniformIndices(shaderProgram, eNumUniformsInBlock, names, indices);

	// get size of all uniform variables in uniform block
	glGetActiveUniformsiv(shaderProgram, eNumUniformsInBlock, indices, GL_UNIFORM_SIZE, size);
	
	// get offsets of all uniform variables in uniform block
	glGetActiveUniformsiv(shaderProgram, eNumUniformsInBlock, indices, GL_UNIFORM_OFFSET, offset);
	
	// get type of all uniform variables in uniform block
	glGetActiveUniformsiv(shaderProgram, eNumUniformsInBlock, indices, GL_UNIFORM_TYPE, type);
	*/
}

void AGN::AShaderPipelineGL::bind()
{
	// TODO: loop through properties and bind them DYNAMICALLY DIFFERENT PER SHADERPIPELINE
	glUseProgram(m_glProgramId);

	if (glGetUniformLocation(m_glProgramId, "uLightAmbient") != -1)
	{
		static const vec4 white(1);
		static const vec4 black(0);
		static const vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
		static const vec4 lightDirection(normalize(vec4(1, 1, 0, 0)));
		glUniform4fv(getUniformIdByName("uLightAmbient"), 1, glm::value_ptr(ambient));
		glUniform4fv(getUniformIdByName("uLightColor"), 1, glm::value_ptr(white));
		glUniform4fv(getUniformIdByName("uLightDirection"), 1, glm::value_ptr(lightDirection));
	}
	
	/*
	// bind shader & its shader specific uniforms
	// TODO: make dynamic
	*/
}

GLint AGN::AShaderPipelineGL::getUniformIdByName(const char* a_name)
{
	GLint uniformID = glGetUniformLocation(m_glProgramId, a_name);

	if (uniformID == -1)
	{
		g_log.error("Uniform: %s == %i", a_name, uniformID);
	}

	return uniformID;

	/*
	// TODO: which is better optimized?
	for (int i = 0; i < m_uniformCount; i++)
	{
		if (m_uniformNames[i].compare(a_name) == 0)
		{
			return m_uniformIDs[i];
		}
	}
	
	g_log.error("Could not find uniform with name: %s", a_name);
	return -1;
	*/
}

/*
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