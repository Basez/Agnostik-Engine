#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "shaderpipeline_gl.hpp"
#include "shader_gl.hpp"
#include "os_utils.hpp"
#include "render_api_gl.hpp"
#include <GL/glew.h>

using namespace glm;

AGN::ShaderPipelineGL::ShaderPipelineGL(const uint32_t a_glprogramId, ShaderPipelineData& a_data)
	: m_aId(a_data.aId)
	, m_glProgramId(a_glprogramId)
{
	// fill shader array
	m_shaders[0] = a_data.vertexShader;
	m_shaders[1] = a_data.pixelShader;
	m_shaders[2] = a_data.hullShader;
	m_shaders[3] = a_data.domainShader;
	m_shaders[4] = a_data.geometryShader;
	m_shaders[5] = a_data.computeShader;

	// get total uniform count
	//glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORMS, &m_uniformPropertyCount);

	// get uniform blocks
	int32_t numBlocks;
	glGetProgramiv(m_glProgramId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
	
	m_constantBuffers.reserve(numBlocks);
	//std::vector<std::string> nameList;
	for (int i = 0; i < numBlocks; i++)
	{
		// get block properties
		int32_t nameLen;
		int32_t blockSize;
		int32_t uniformCount;
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniformCount);

		// get blockname
		char blockName[ShaderPipelineGL::MAX_UNIFORM_NAME];
		glGetActiveUniformBlockName(m_glProgramId, i, ShaderPipelineGL::MAX_UNIFORM_NAME, NULL, blockName);

		// get index
		int32_t index = glGetUniformBlockIndex(m_glProgramId, blockName);


		// create object
		ConstantBufferGL* constantBuffer = new ConstantBufferGL();
		AGN::cStringCopy(constantBuffer->name, blockName, sizeof(blockName));
		constantBuffer->index = index;
		constantBuffer->size = blockSize;

		// get uniform indices
		constantBuffer->uniformProperty = uniformCount;

		// get uniform properties


		int32_t* uniformIds = new int32_t[uniformCount];
		int32_t* uniformOffsets = new int32_t[uniformCount];
		glGetActiveUniformBlockiv(m_glProgramId, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIds);
		glGetActiveUniformsiv(m_glProgramId, (GLsizei)uniformCount, (uint32_t*)uniformIds, GL_UNIFORM_OFFSET, uniformOffsets);

		// create and fill propertyList 
		constantBuffer->propertyList.reserve(uniformCount);

		// get uniform names
		for (int32_t j = 0; j < uniformCount; j++)
		{
			ConstantBufferPropertyGL* uniformProperty = new ConstantBufferPropertyGL();

			// get uniform name
			char uniformName[ShaderPipelineGL::MAX_UNIFORM_NAME];
			int32_t length;
			glGetActiveUniformName(m_glProgramId, uniformIds[j], ShaderPipelineGL::MAX_UNIFORM_NAME, &length, uniformName);
			
			// fill constant buffer uniform properties
			memcpy(uniformProperty->name, uniformName, ShaderPipelineGL::MAX_UNIFORM_NAME);
			uniformProperty->id = uniformIds[j];
			uniformProperty->offset = uniformOffsets[j];

			constantBuffer->propertyList.push_back(uniformProperty);
		}

		// create & bind buffer
		glGenBuffers(1, &constantBuffer->uboHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, constantBuffer->uboHandle);

		// Bind the buffer object to the uniform block.
		glBindBufferBase(GL_UNIFORM_BUFFER, index, constantBuffer->uboHandle);

		// upload (null at the moment) data
		glBufferData(GL_UNIFORM_BUFFER, constantBuffer->size, 0, GL_DYNAMIC_DRAW);
		
		// unbind & clean
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		delete[] uniformIds;
		delete[] uniformOffsets;

		m_constantBuffers.push_back(constantBuffer);
	}

	AGN::RenderAPIGL::getOpenGLErrors();
}

AGN::ShaderPipelineGL::~ShaderPipelineGL()
{	
	while (m_constantBuffers.size() > 0)
	{
		ConstantBufferGL* constantBuffer = m_constantBuffers[0];
		// cleanup inner property list
		while (constantBuffer->propertyList.size() > 0)
		{
			delete constantBuffer->propertyList[0];
			constantBuffer->propertyList.erase(constantBuffer->propertyList.begin());
		}

		// delete struct itself
		delete constantBuffer;
		m_constantBuffers.erase(m_constantBuffers.begin());
	}
}

AGN::IShader* AGN::ShaderPipelineGL::getShader(const EShaderType a_type)
{
	const uint32_t numShaders = sizeof(m_shaders) / sizeof(m_shaders[0]);

	for (uint32_t i = 0; i < numShaders; i++)
	{
		if (m_shaders[i]->getType() == a_type) return m_shaders[i];
	}

	g_log.error("Shader with type not recognized or added");
	assert(false);
	return nullptr;
}

void AGN::ShaderPipelineGL::bind()
{
	glUseProgram(m_glProgramId);

	// TODO: do this based on if its a per object/perframe/per application Constant buffer???
	for (unsigned int i = 0; i < m_constantBuffers.size(); i++)
	{
		// bind & upload buffer
		glBindBuffer(GL_UNIFORM_BUFFER, m_constantBuffers[i]->uboHandle);
	}

	AGN::RenderAPIGL::getOpenGLErrors();
}

int32_t AGN::ShaderPipelineGL::getUniformIdByName(const char* a_name)
{
	int32_t uniformID = glGetUniformLocation(m_glProgramId, a_name);

	if (uniformID == -1)
	{
		g_log.error("Uniform: %s == %i", a_name, uniformID);
	}

	return uniformID;
}

bool AGN::ShaderPipelineGL::hasUniform(const char* a_name)
{
	return glGetUniformLocation(m_glProgramId, a_name) != -1;
}

void AGN::ShaderPipelineGL::setConstantBufferData(const EShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize)
{
	ConstantBufferGL* uniformConstantBuffer = getUniformConstantBufferByName(a_name);

	// upload buffer
	glNamedBufferData(uniformConstantBuffer->uboHandle, uniformConstantBuffer->size, a_data, GL_DYNAMIC_DRAW);
}

bool AGN::ShaderPipelineGL::hasConstantBuffer(const EShaderType a_shader, const char* a_name)
{
	for (unsigned int i = 0; i < m_constantBuffers.size(); i++)
	{
		if (strcmp(m_constantBuffers[i]->name, a_name) == 0) return true;
	}

	return false;
}

struct AGN::ConstantBufferGL* AGN::ShaderPipelineGL::getUniformConstantBufferByName(const char* a_name)
{
	for (unsigned int i = 0; i < m_constantBuffers.size(); i++)
	{
		if (strcmp(m_constantBuffers[i]->name, a_name) == 0)
			return m_constantBuffers[i];
	}

	assert(false);
	return nullptr;
}

AGN::ConstantBufferPropertyGL* AGN::ConstantBufferGL::getPropertyByName(const char* a_name)
{
	for (int32_t i = 0; i < uniformProperty; i++)
	{
		if (strcmp(propertyList[i]->name, a_name) == 0)
		{
			return propertyList[i];
		}
	}

	g_log.error("Constant buffer uniform property with name %s was not found in buffer %s", a_name, name);
	return nullptr;
}
