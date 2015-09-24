#include "asharedh.hpp"
#include "ashaderpipeline_gl.hpp"
#include "ashader_gl.hpp"

using namespace glm;

GLubyte* g_blockBuffer;
GLuint g_uboHandle;
GLuint g_blockIndex;
GLint g_blockSize;

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
	glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORMS, &m_uniformPropertyCount);


	g_blockIndex = glGetUniformBlockIndex(m_glProgramId, "LightSettings");

	glGetActiveUniformBlockiv(m_glProgramId, g_blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &g_blockSize);

	g_blockBuffer = (GLubyte *)malloc(g_blockSize);

	// Query for the offsets of each block variable
	const GLchar *names[] = { "uLightDirection", "uLightColor", "uLightAmbient" };

	GLuint indices[3];
	glGetUniformIndices(m_glProgramId, 3, names, indices);

	GLint offset[3];
	glGetActiveUniformsiv(m_glProgramId, 3, indices, GL_UNIFORM_OFFSET, offset);


	//static const vec4 white(1);
	//static const vec4 black(0);
	//static const vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
	static const vec4 lightDirectionNorm(normalize(vec4(1, 1, 1, 1)));
	//glUniform4fv(getUniformIdByName("uLightAmbient"), 1, glm::value_ptr(ambient));
	//glUniform4fv(getUniformIdByName("uLightColor"), 1, glm::value_ptr(white));
	//glUniform4fv(getUniformIdByName("uLightDirection"), 1, glm::value_ptr(lightDirection));

	float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	float lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightDirection[] = { lightDirectionNorm[0], lightDirectionNorm[1], lightDirectionNorm[2], lightDirectionNorm[3] };

	memcpy(g_blockBuffer + offset[0], lightDirection,4 * sizeof(float));
	memcpy(g_blockBuffer + offset[1], lightColor, 4 * sizeof(float));
	memcpy(g_blockBuffer + offset[2], lightAmbient, 4 * sizeof(float));

	// Create the OpenGL buffer object and copy the data into it.
	glGenBuffers(1, &g_uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, g_uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, g_blockSize, g_blockBuffer, GL_DYNAMIC_DRAW);

	// Bind the buffer object to the uniform block.
	glBindBufferBase(GL_UNIFORM_BUFFER, g_blockIndex, g_uboHandle);

	AGN::getOpenGLError();

	//void glGetUniformIndices?(GLuint program?, GLsizei uniformCount?, const char ** uniformNames?, GLuint *uniformIndices?);
	//void glGetActiveUniformName?(GLuint program?, GLuint uniformIndex?, GLsizei bufSize?, GLsizei *length?, char *uniformName?);
	//void glGetActiveUniformsiv?(GLuint program?, GLsizei uniformCount?, const GLuint *uniformIndices?, GLenum pname?, GLint *params?);

	// retrieve all uniform names
	/*
	for (int i = 0; i < m_uniformPropertyCount; i++)
	{
		// get name first
		GLsizei uniformNameLength = -1;
		GLchar uniformName[MAX_UNIFORM_NAME];
		glGetActiveUniformName(m_glProgramId, i, MAX_UNIFORM_NAME, &uniformNameLength, uniformName);
		
		assert(uniformNameLength < MAX_UNIFORM_NAME);

		// get ID
		GLint uniformId = glGetUniformLocation(m_glProgramId, uniformName);

		// get type and other properties
		int uniformSize = 0;
		GLenum uniformType = 0;
		glGetActiveUniform(m_glProgramId, uniformId, MAX_UNIFORM_NAME, &uniformNameLength, &uniformSize, &uniformType, uniformName);

		// create pod uniform struct
		AUniformPropertyGL uniformProperty;
		memset(&uniformProperty.data, 0, sizeof(uniformProperty));		// set data to 0 (null)
		uniformProperty.id = uniformId;
		uniformProperty.type = uniformType;
		uniformProperty.isStatic = false; // assume value is dynamic, as soon as user sets a data value it is set to true;
		strcpy(uniformProperty.name, uniformName);

		switch (uniformType)
		{
		// supported types
		case GL_INT:
		case GL_FLOAT:
		case GL_FLOAT_VEC2:
		case GL_FLOAT_VEC3:
		case GL_FLOAT_VEC4:
		case GL_SAMPLER_2D:
		case GL_FLOAT_MAT4:
		case GL_FLOAT_MAT3:
			uniformProperty.type = EAShaderPipelineDataType::
			m_uniformProperties.push_back(uniformProperty);
			break;

		default:
			break;
		}

		int asda = 1;
		
	}
	*/

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

	// Bind the buffer object to the uniform block.
	//glBindBuffer(GL_UNIFORM_BUFFER, g_uboHandle);
	//glBufferData(GL_UNIFORM_BUFFER, g_blockSize, g_blockBuffer, GL_DYNAMIC_DRAW);

	//glBindBufferBase(GL_UNIFORM_BUFFER, g_blockIndex, g_uboHandle);

	/*
	if (glGetUniformLocation(m_glProgramId, "uLightAmbient") != -1)
	{
		static const vec4 white(1);
		static const vec4 black(0);
		static const vec4 ambient(0.2f, 0.2f, 0.2f, 1.0f);
		static const vec4 lightDirection(normalize(vec4(1, 1, 1, 1)));
		glUniform4fv(getUniformIdByName("uLightAmbient"), 1, glm::value_ptr(ambient));
		glUniform4fv(getUniformIdByName("uLightColor"), 1, glm::value_ptr(white));
		glUniform4fv(getUniformIdByName("uLightDirection"), 1, glm::value_ptr(lightDirection));
	}
	*/

	/*
	// bind shader & its shader specific uniforms
	// TODO: make dynamic
	*/
}

void AGN::AShaderPipelineGL::setStaticUniformFloat(const char* a_name, float a_value)
{
	//m_uniformProperties[getUniformIndexByNameAndType(a_name, GL_FLOAT)].data.asFloat = a_value;
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
int AGN::AShaderPipelineGL::getUniformIndexByNameAndType(const char* a_name, GLenum a_type)
{
	for (int i = 0; i < m_uniformProperties.size(); i++)
	{
		if (strcmp(m_uniformProperties[i].name, a_name) == 0 && a_type == m_uniformProperties[i].type)
		{
			return i;
		}
	}
	assert(false);
	return -1;
}
*/

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