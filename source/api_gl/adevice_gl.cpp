#include "asharedh.hpp"
#include "adevice_gl.hpp"
#include "iamesh.hpp"
#include "amesh_gl.hpp"


// glew
#include <GL/glew.h>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

using namespace glm;

AGN::ADeviceGL::ADeviceGL()
{

}

void AGN::ADeviceGL::init()
{
	
}

AGN::IAMesh* AGN::ADeviceGL::createMesh(AGN::MeshData& a_meshData)
{
	// upload the data to the GL Driver and GFX card
	uint32_t vao = -1;
	uint8_t vboCount = 4;
	uint32_t *vbos = new uint32_t[4]();
	GLenum errorType = GL_NO_ERROR;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(vboCount, vbos);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData.positions.size() * sizeof(vec3), a_meshData.positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::AMeshGLAttribute::MESH_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::AMeshGLAttribute::MESH_POSITION_ATTRIBUTE);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData.normals.size() * sizeof(vec3), a_meshData.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::AMeshGLAttribute::MESH_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::AMeshGLAttribute::MESH_NORMAL_ATTRIBUTE);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData.textureCoords.size() * sizeof(vec2), a_meshData.textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::AMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::AMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_meshData.indicies.size() * sizeof(GLuint), a_meshData.indicies.data(), GL_STATIC_DRAW);

	// unbind because we are done
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Check if everything went allright
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An error occurred during AMeshGL initialization: %u", errorType);
	}

	// instantiate Mesh Object with pointers to the uploaded data
	AMeshGL* mesh = new AMeshGL(vao, vbos, vboCount);

	return dynamic_cast<IAMesh*>(mesh);
}

/*
void Mesh::generateShaders()
{
	GLuint vertexShader = g_shaderManager.loadShaderFromString(GL_VERTEX_SHADER, g_shader_mesh_vert);
	GLuint fragmentShader = g_shaderManager.loadShaderFromString(GL_FRAGMENT_SHADER, g_shader_mesh_frag);

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	m_shaderProgram = g_shaderManager.createShaderProgram(shaders);
	assert(m_shaderProgram);

	m_uniformMVP = ShaderManager::getUniformLocation(m_shaderProgram, "uModelViewProjectionMatrix");
	m_uniformModelMatrix = ShaderManager::getUniformLocation(m_shaderProgram, "uModelMatrix");
	m_uniformClipPlane = ShaderManager::getUniformLocation(m_shaderProgram, "uClipPlane");
	m_uniformClipOffset = ShaderManager::getUniformLocation(m_shaderProgram, "uClipOffset");

	// Light properties.
	m_uniformLightDirection = ShaderManager::getUniformLocation(m_shaderProgram, "uLightDirection");
	m_uniformLightColor = ShaderManager::getUniformLocation(m_shaderProgram, "uLightColor");

	// Global ambient.
	m_uniformAmbient = ShaderManager::getUniformLocation(m_shaderProgram, "uLightAmbient");

	// Material properties.
	m_uniformMaterialEmissive = ShaderManager::getUniformLocation(m_shaderProgram, "uMaterialEmissive");
	m_uniformMaterialDiffuse = ShaderManager::getUniformLocation(m_shaderProgram, "uMaterialDiffuse");
	//m_uniformMaterialSpecular = ShaderManager::getUniformLocation(m_shaderProgram, "uMaterialSpecular");
	//m_uniformMaterialShininess = ShaderManager::getUniformLocation(m_shaderProgram, "uMaterialShininess");
}
*/