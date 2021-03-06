#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "device_gl.hpp"
#include "mesh_gl.hpp"
#include "texture_gl.hpp"
#include "shader_gl.hpp"
#include "shaderpipeline_gl.hpp"
#include "render_api_gl.hpp"

#include <GL/glew.h>

using namespace glm;

// used for openGL's VBO initializations
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))

AGN::DeviceGL::DeviceGL()
{

}

AGN::DeviceGL::~DeviceGL()
{
	
}

bool AGN::DeviceGL::init()
{
	return true;
}

void AGN::DeviceGL::onWindowUpdated(glm::ivec2 a_dimensions)
{
	
}

AGN::IMesh* AGN::DeviceGL::createMesh(const uint16_t a_aId, AGN::MeshData* a_meshData)
{
	// upload the data to the GL Driver and GFX card
	uint32_t vao = -1;
	const uint8_t vboCount = 6;
	uint32_t *vbos = new uint32_t[vboCount]();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(vboCount, vbos);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->positions.size() * sizeof(vec3), a_meshData->positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->normals.size() * sizeof(vec3), a_meshData->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE), 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->textureCoords.size() * sizeof(vec2), a_meshData->textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE), 2, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->tangents.size() * sizeof(vec3), a_meshData->tangents.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_TANGENT_ATTRIBUTE), 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_TANGENT_ATTRIBUTE));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[4]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->bitangents.size() * sizeof(vec3), a_meshData->bitangents.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_BITANGENT_ATTRIBUTE), 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(static_cast<int>(MeshGL::EAMeshGLAttribute::MESH_BITANGENT_ATTRIBUTE));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_meshData->indicies.size() * sizeof(uint32_t), a_meshData->indicies.data(), GL_STATIC_DRAW);

	// unbind because we are done
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Check if everything went all right
	AGN::RenderAPIGL::getOpenGLErrors();

	// instantiate Mesh Object with pointers to the uploaded data
	MeshGL* mesh = new MeshGL(a_aId, vao, vbos, vboCount, a_meshData);

	return dynamic_cast<IMesh*>(mesh);
}

AGN::ITexture* AGN::DeviceGL::createTexture(const uint16_t a_aId, AGN::TextureData* a_textureData)
{
	GLenum glType = TextureGL::getGlTypeByTextureType(a_textureData->type);

	uint32_t textureID = -1;

	// generate GL texture
	if (glType == GL_TEXTURE_2D)
	{
		// get max mipmap level
		const int maxResolution = max(a_textureData->width, a_textureData->height);
		const int numMipmaps = static_cast<int>(floor(log2((float)maxResolution)));

		// generate texture with mipmaps
		glGenTextures(1, &textureID);
		glBindTexture(glType, textureID);
		glTexStorage2D(glType, numMipmaps, GL_RGBA8, a_textureData->width, a_textureData->height);
		glTexSubImage2D(glType, 0, 0, 0, a_textureData->width, a_textureData->height, GL_RGBA, GL_UNSIGNED_BYTE, a_textureData->buffer);
		glGenerateMipmap(glType);  //Generate num_mipmaps number of mipmaps here.

		AGN::RenderAPIGL::getOpenGLErrors();

		// set parameters
		if (a_textureData->flags & (int)ETextureRenderFlags::USE_NEAREST_NEIGBOR)
		{
			glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if (a_textureData->flags & (int)ETextureRenderFlags::USE_LINEAR)
		{
			glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			// DEFAULT IS MIPMAPS
			glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (a_textureData->flags & (int)ETextureRenderFlags::USE_CLAMP)
		{
			glTexParameteri(glType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(glType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			// DEFAULT IS REPEAT
			glTexParameteri(glType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(glType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		AGN::RenderAPIGL::getOpenGLErrors();

		// anisotropic filtering
		float maxAnisotropy = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(glType, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

		glBindTexture(glType, 0);
	}
	else
	{
		g_log.error("Unsupported TextureType: %X please add support", glType);
	}

	AGN::RenderAPIGL::getOpenGLErrors();

	// create actual texture.
	TextureGL* texture = new TextureGL(a_aId, a_textureData, textureID);

	return dynamic_cast<ITexture*>(texture);
}

AGN::IShader* AGN::DeviceGL::createShader(const uint16_t a_aId, const char* a_shaderSource, AGN::EShaderType a_type)
{
	// generate gl shader
	uint32_t shaderGlId = glCreateShader(ShaderGL::getGlShaderType(a_type));

	// Load the shader source for each shader object.
	const char* sources[] = { a_shaderSource };
	glShaderSource(shaderGlId, 1, sources, NULL);
	glCompileShader(shaderGlId);

	// Check for errors
	int32_t compileStatus;
	glGetShaderiv(shaderGlId, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		int32_t logLength;
		glGetShaderiv(shaderGlId, GL_INFO_LOG_LENGTH, &logLength);
		char* infoLog = new char[logLength];
		glGetShaderInfoLog(shaderGlId, logLength, NULL, infoLog);

		g_log.error("Error during shader parsing: %s", infoLog);

		delete infoLog;

		g_log.error("something went wrong with loading / compiling shader");

		return nullptr;
	}
	ShaderGL* shader = new ShaderGL(a_aId, a_type, shaderGlId);

	return dynamic_cast<IShader*>(shader);
}

AGN::IShaderPipeline* AGN::DeviceGL::createShaderPipeline(const uint16_t a_aId, std::vector<AGN::IShader*> a_shaders)
{
	ShaderPipelineData shaderPipelineData = ShaderPipelineData();
	shaderPipelineData.aId = a_aId;

	uint32_t programGl = glCreateProgram();

	for (unsigned int i = 0; i < a_shaders.size(); i++)
	{
		ShaderGL* shaderGL = dynamic_cast<ShaderGL*>(a_shaders[i]);
		glAttachShader(programGl, shaderGL->getGlId());

		switch (shaderGL->getType())
		{
		case EShaderType::VertexShader:
			shaderPipelineData.vertexShader = a_shaders[i];
			break;

		case EShaderType::PixelShader:
			shaderPipelineData.pixelShader = a_shaders[i];
			break;

		case EShaderType::HullShader:
			shaderPipelineData.hullShader = a_shaders[i];
			break;

		case EShaderType::DomainShader:
			shaderPipelineData.domainShader = a_shaders[i];
			break;

		case EShaderType::GeometryShader:
			shaderPipelineData.geometryShader = a_shaders[i];
			break;

		case EShaderType::ComputeShader:
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
	int32_t linkStatus;
	glGetProgramiv(programGl, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		int32_t logLength;
		glGetProgramiv(programGl, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(programGl, logLength, NULL, infoLog);

		g_log.error(infoLog);

		delete infoLog;
		return nullptr;
	}
	
	if (programGl == (uint32_t)-1)
	{
		g_log.error("something went wrong with loading / compiling shader");
		return nullptr;
	}

	ShaderPipelineGL* shaderPipeline = new ShaderPipelineGL(programGl, shaderPipelineData);

	return dynamic_cast<IShaderPipeline*>(shaderPipeline);
}