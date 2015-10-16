#include "asharedh.hpp"
#include "asharedapi.hpp" // Glew & OpenGL
#include "adevice_gl.hpp"
#include "amesh_gl.hpp"
#include "atexture_gl.hpp"
#include "ashader_gl.hpp"
#include "ashaderpipeline_gl.hpp"

using namespace glm;

AGN::ADeviceGL::ADeviceGL()
{

}

bool AGN::ADeviceGL::init()
{
	return true;
}

AGN::IAMesh* AGN::ADeviceGL::createMesh(const uint16_t a_aId, AGN::AMeshData* a_meshData)
{
	// upload the data to the GL Driver and GFX card
	uint32_t vao = -1;
	const uint8_t vboCount = 4;
	uint32_t *vbos = new uint32_t[vboCount]();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(vboCount, vbos);

	
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->positions.size() * sizeof(vec3), a_meshData->positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->normals.size() * sizeof(vec3), a_meshData->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE), 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->textureCoords.size() * sizeof(vec2), a_meshData->textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(AMeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_meshData->indicies.size() * sizeof(GLuint), a_meshData->indicies.data(), GL_STATIC_DRAW);

	// unbind because we are done
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Check if everything went all right
	AGN::getOpenGLError();

	// instantiate Mesh Object with pointers to the uploaded data
	AMeshGL* mesh = new AMeshGL(a_aId, vao, vbos, vboCount, a_meshData);

	return dynamic_cast<IAMesh*>(mesh);
}

AGN::IATexture* AGN::ADeviceGL::createTexture(const uint16_t a_aId, AGN::ATextureData* a_textureData)
{
	GLenum glType = ATextureGL::getGlTypeByTextureType(a_textureData->type);

	GLuint textureID = -1;

	// generate GL texture
	if (glType == GL_TEXTURE_2D)
	{
		glGenTextures(1, &textureID);    // get new texture ID
		glBindTexture(glType, textureID);
		glTexImage2D(glType, 0, GL_RGBA, a_textureData->width, a_textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, a_textureData->buffer);
		glBindTexture(glType, 0);
	}
	else
	{
		g_log.error("Unsupported TextureType: %X please add support", glType);
	}
	

	AGN::getOpenGLError();

	// create actual texture.
	ATextureGL* texture = new ATextureGL(a_aId, a_textureData, textureID);

	return dynamic_cast<IATexture*>(texture);
}

AGN::IAShader* AGN::ADeviceGL::createShader(const uint16_t a_aId, const char* a_shaderSource, AGN::EAShaderType a_type)
{
	// generate gl shader
	GLuint shaderGlId = glCreateShader(AShaderGL::getGlShaderType(a_type));

	// Load the shader source for each shader object.
	const GLchar* sources[] = { a_shaderSource };
	glShaderSource(shaderGlId, 1, sources, NULL);
	glCompileShader(shaderGlId);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(shaderGlId, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shaderGlId, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shaderGlId, logLength, NULL, infoLog);

		g_log.error("Error during shader parsing: %s", infoLog);

		delete infoLog;

		g_log.error("something went wrong with loading / compiling shader");

		return nullptr;
	}
	AShaderGL* shader = new AShaderGL(a_aId, a_type, shaderGlId);

	return dynamic_cast<IAShader*>(shader);
}

AGN::IAShaderPipeline* AGN::ADeviceGL::createShaderPipeline(const uint16_t a_aId, std::vector<AGN::IAShader*> a_shaders)
{
	AShaderPipelineData shaderPipelineData = AShaderPipelineData();
	shaderPipelineData.aId = a_aId;

	GLuint programGl = glCreateProgram();

	for (unsigned int i = 0; i < a_shaders.size(); i++)
	{
		AShaderGL* shaderGL = dynamic_cast<AShaderGL*>(a_shaders[i]);
		glAttachShader(programGl, shaderGL->getGlId());

		switch (shaderGL->getType())
		{
		case EAShaderType::VertexShader:
			shaderPipelineData.vertexShader = a_shaders[i];
			break;

		case EAShaderType::PixelShader:
			shaderPipelineData.pixelShader = a_shaders[i];
			break;

		case EAShaderType::HullShader:
			shaderPipelineData.hullShader = a_shaders[i];
			break;

		case EAShaderType::DomainShader:
			shaderPipelineData.domainShader = a_shaders[i];
			break;

		case EAShaderType::GeometryShader:
			shaderPipelineData.geometryShader = a_shaders[i];
			break;

		case EAShaderType::ComputeShader:
			shaderPipelineData.computeShader = a_shaders[i];
			break;
		}
	}

	if (shaderPipelineData.pixelShader == nullptr || shaderPipelineData.vertexShader == nullptr)
	{
		g_log.warning("ShaderPipeline is missing a pixelShader or vertexShader");
	}

	// Link the program
	glLinkProgram(programGl);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(programGl, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(programGl, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(programGl, logLength, NULL, infoLog);

		g_log.error(infoLog);

		delete infoLog;
		return nullptr;
	}

	if (programGl == (GLuint)-1)
	{
		g_log.error("something went wrong with loading / compiling shader");
		return nullptr;
	}

	AShaderPipelineGL* shaderPipeline = new AShaderPipelineGL(programGl, shaderPipelineData);

	return dynamic_cast<IAShaderPipeline*>(shaderPipeline);
}