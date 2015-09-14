#include "asharedh.hpp"
#include "adevice_gl.hpp"
#include "amesh_gl.hpp"
#include "atexture_gl.hpp"
#include "ashader_gl.hpp"
#include "ashaderprogram_gl.hpp"

using namespace glm;

AGN::ADeviceGL::ADeviceGL()
{

}

void AGN::ADeviceGL::init()
{
	
}

AGN::IAMesh* AGN::ADeviceGL::createMesh(AGN::AMeshData* a_meshData)
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
	glBufferData(GL_ARRAY_BUFFER, a_meshData->positions.size() * sizeof(vec3), a_meshData->positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::EAMeshGLAttribute::MESH_POSITION_ATTRIBUTE);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->normals.size() * sizeof(vec3), a_meshData->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::EAMeshGLAttribute::MESH_NORMAL_ATTRIBUTE);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, a_meshData->textureCoords.size() * sizeof(vec2), a_meshData->textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(AMeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(AMeshGL::EAMeshGLAttribute::MESH_TEXCOORD_ATTRIBUTE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_meshData->indicies.size() * sizeof(GLuint), a_meshData->indicies.data(), GL_STATIC_DRAW);

	// unbind because we are done
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Check if everything went all right
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An error occurred during AMeshGL initialization: %u", errorType);
	}

	// instantiate Mesh Object with pointers to the uploaded data
	AMeshGL* mesh = new AMeshGL(a_meshData, vao, vbos, vboCount);

	return dynamic_cast<IAMesh*>(mesh);
}

AGN::IATexture* AGN::ADeviceGL::createTexture(AGN::ATextureData* a_textureData)
{
	GLenum glType = ATextureGL::getGlType(a_textureData->type);

	GLuint textureID = -1;

	// generate GL texture
	glGenTextures(1, &textureID);    // get new texture ID
	glBindTexture(glType, textureID);
	glTexImage2D(glType, 0, GL_RGBA, a_textureData->width, a_textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, a_textureData->buffer);
	glBindTexture(glType, 0);

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred in ADeviceGL::createTexture(): %s ", AConversionUtils::getAsHexString(errorType).c_str());
	}

	// create actual texture.
	ATextureGL* texture = new ATextureGL(a_textureData, textureID);

	return dynamic_cast<IATexture*>(texture);
}

AGN::IAShader* AGN::ADeviceGL::createShader(const char* a_shaderSource, AGN::EAShaderType a_type)
{
	// generate gl shader
	GLuint shaderID = glCreateShader(AShaderGL::getGlShaderType(a_type));

	// Load the shader source for each shader object.
	const GLchar* sources[] = { a_shaderSource };
	glShaderSource(shaderID, 1, sources, NULL);
	glCompileShader(shaderID);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, infoLog);

		g_log.error(infoLog);

		delete infoLog;

		g_log.error("something went wrong with loading / compiling shader");

		return nullptr;
	}
	AShaderGL *shader = new AShaderGL(a_type, shaderID);

	return dynamic_cast<IAShader*>(shader);
}

AGN::IAShaderProgram* AGN::ADeviceGL::createShaderProgram(std::vector<AGN::IAShader*> a_shaders)
{
	// create the GL program
	GLuint program = glCreateProgram();

	for (unsigned int i = 0; i < a_shaders.size(); i++)
	{
		AShaderGL* shaderGL = dynamic_cast<AShaderGL*>(a_shaders[i]);
		glAttachShader(program, shaderGL->getGlID());
	}

	// Link the program
	glLinkProgram(program);
 
	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus != GL_TRUE )
	{
		GLint logLength;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
		GLchar* infoLog = new GLchar[logLength];
 
		glGetProgramInfoLog( program, logLength, NULL, infoLog );
 
		g_log.error(infoLog);

		delete infoLog;
		return nullptr;
	}

	if (program == (GLuint)-1) 
	{
		g_log.error("something went wrong with loading / compiling shader");
		return nullptr;
	}

	AShaderProgramGL* shaderProgram = new AShaderProgramGL(program, a_shaders);

	return dynamic_cast<IAShaderProgram*>(shaderProgram);
}