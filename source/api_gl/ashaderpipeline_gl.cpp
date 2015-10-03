#include "asharedh.hpp"
#include "ashaderpipeline_gl.hpp"
#include "ashader_gl.hpp"
#include "aosutils.hpp"

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
	//glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORMS, &m_uniformPropertyCount);

	// get uniform blocks
	GLint numBlocks;
	glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
	
	m_uniformBuffers.reserve(numBlocks);
	//std::vector<std::string> nameList;
	for (int i = 0; i < numBlocks; i++)
	{
		// get block properties
		GLint nameLen;
		GLint blockSize;
		GLint uniformCount;
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

		// get blockname
		char blockName[MAX_UNIFORM_NAME];
		glGetActiveUniformBlockName(m_glProgramId, i, MAX_UNIFORM_NAME, NULL, blockName);

		// get index
		GLint index = glGetUniformBlockIndex(m_glProgramId, blockName);

		// TODO: get uniform names
		//glGetActiveUniformName()

		// create object
		AUniformConstantBufferGL* constantBuffer = new AUniformConstantBufferGL();
		AGN::cStringCopy(constantBuffer->name, blockName, sizeof(blockName));
		constantBuffer->index = index;
		constantBuffer->size = blockSize;

		// create the data & null it
		constantBuffer->buffer = (GLubyte *)malloc(blockSize);
		memset(constantBuffer->buffer, 0, blockSize);

		// get uniform indices
		constantBuffer->uniformCount = uniformCount;
		constantBuffer->uniformIds = new GLint[uniformCount];
		constantBuffer->uniformOffsets = new GLint[uniformCount];
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, constantBuffer->uniformIds);
		glGetActiveUniformsiv(m_glProgramId, (GLsizei)uniformCount, (GLuint*)constantBuffer->uniformIds, GL_UNIFORM_OFFSET, constantBuffer->uniformOffsets);

		// create & bind buffer
		glGenBuffers(1, &constantBuffer->uboHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, constantBuffer->uboHandle);

		// Bind the buffer object to the uniform block.
		glBindBufferBase(GL_UNIFORM_BUFFER, index, constantBuffer->uboHandle);

		// upload (null at the moment) data
		glBufferData(GL_UNIFORM_BUFFER, constantBuffer->size, constantBuffer->buffer, GL_DYNAMIC_DRAW);
		
		// unbind
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		m_uniformBuffers.push_back(constantBuffer);
	}

	AGN::getOpenGLError();
}

void AGN::AShaderPipelineGL::bind()
{
	glUseProgram(m_glProgramId);

	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		// bind & upload buffer
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffers[i]->uboHandle);
		glBufferData(GL_UNIFORM_BUFFER, m_uniformBuffers[i]->size, m_uniformBuffers[i]->buffer, GL_DYNAMIC_DRAW);
	}

	AGN::getOpenGLError();
}

GLint AGN::AShaderPipelineGL::getUniformIdByName(const char* a_name)
{
	GLint uniformID = glGetUniformLocation(m_glProgramId, a_name);

	if (uniformID == -1)
	{
		g_log.error("Uniform: %s == %i", a_name, uniformID);
	}

	return uniformID;
}

bool AGN::AShaderPipelineGL::hasUniform(const char* a_name)
{
	return glGetUniformLocation(m_glProgramId, a_name) != -1;
}

void AGN::AShaderPipelineGL::setUniformBufferData(const char* a_name, void* a_data, size_t a_dataSize)
{
	AUniformConstantBufferGL* uniformConstantBuffer = getUniformConstantBufferByName(a_name);

	memcpy(uniformConstantBuffer->buffer, a_data, a_dataSize);

	// bind & upload buffer
	glBindBuffer(GL_UNIFORM_BUFFER, uniformConstantBuffer->uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, uniformConstantBuffer->size, uniformConstantBuffer->buffer, GL_DYNAMIC_DRAW);
}

bool AGN::AShaderPipelineGL::hasUniformBuffer(const char* a_name)
{
	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (strcmp(m_uniformBuffers[i]->name, a_name) == 0) return true;
	}

	return false;
}

struct AGN::AUniformConstantBufferGL* AGN::AShaderPipelineGL::getUniformConstantBufferByName(const char* a_name)
{
	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (strcmp(m_uniformBuffers[i]->name, a_name) == 0)
			return m_uniformBuffers[i];
	}

	assert(false);
	return nullptr;
}
