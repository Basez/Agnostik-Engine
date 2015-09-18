#include "asharedh.hpp"
#include "atexture_gl.hpp"

AGN::ATextureGL::ATextureGL(const uint16_t a_id, ATextureData* a_textureData, GLenum a_textureID)
	: m_id(a_id)
	, m_textureData(a_textureData)
	, m_textureID(a_textureID)
{
	m_glType = ATextureGL::getGlType(a_textureData->type);
	setTextureParams(a_textureData->flags);

	// upload to gpu
	if (m_textureData->buffer != nullptr)
	{
		pushBuffer();
	}
}

std::string AGN::ATextureGL::getRelativePath()
{
	return m_textureData->relativePath;
}

void AGN::ATextureGL::setTextureParams(unsigned int a_flags)
{
	glBindTexture(m_glType, m_textureID);

	// set parameters
	if (a_flags & (int)EATextureRenderFlags::USE_NEAREST_NEIGBOR)
	{
		glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (a_flags & (int)EATextureRenderFlags::USE_MIP_MAPS)
	{
		g_log.error("TODO: implement mip maps support for atexture_gl.cpp");

		// generate mip maps
		gluBuild2DMipmaps(m_glType, GL_RGBA, m_textureData->width, m_textureData->height, GL_RGBA, GL_UNSIGNED_BYTE, NULL/*a_image.pixels*/);	// TODO:!!!!!!!!!!!!!!
		glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		// DEFAULT IS LINEAR
		glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (a_flags & (int)EATextureRenderFlags::USE_CLAMP)
	{
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		// DEFAULT IS REPEAT
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glBindTexture(m_glType, 0);

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred in ATextureGL::setTextureParams(): %s ", AConversionUtils::getAsHexString(errorType).c_str());
	}
}


// pull buffer data from GPU into CPU
void AGN::ATextureGL::pullBuffer()
{
	// delete old buffer first
	if (m_textureData->buffer != nullptr)
	{
		delete m_textureData->buffer;
		m_textureData->buffer = nullptr;
	}

	// get buffer from loaded texture
	glBindTexture(m_glType, m_textureID);
	const unsigned int size = m_textureData->width * m_textureData->height;
	m_textureData->buffer = new uint32_t[size];
	glGetTexImage(m_glType, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData->buffer);
	glBindTexture(m_glType, 0);

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred in ATextureGL::pullBuffer(): %s ", AConversionUtils::getAsHexString(errorType).c_str());
	}
}

// generates a new texture on the GPU memory with the currently stored pixeldata
// TODO: use glTexSubImage2D instead, should be faster, but cnt get it to work yet
void AGN::ATextureGL::pushBuffer()
{
	if (m_textureData->buffer == nullptr)
	{
		g_log.error("Cannot push buffer of this texture, because the buffer is null!");
	}

	glBindTexture(m_glType, m_textureID);
	glTexImage2D(m_glType, 0, GL_RGBA, m_textureData->width, m_textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData->buffer);
	glBindTexture(m_glType, 0);

	GLenum errorType = GL_NO_ERROR;
	while ((errorType = glGetError()) != GL_NO_ERROR)
	{
		g_log.error("An OpenGL error occurred in ATextureGL::pushBuffer(): %s ", AConversionUtils::getAsHexString(errorType).c_str());
	}
}

GLenum AGN::ATextureGL::getGlType(EATextureType a_type)
{
	switch (a_type)
	{
	case EATextureType::TEXTURE_1D:
		return GL_TEXTURE_1D;

	case EATextureType::TEXTURE_2D:
		return GL_TEXTURE_2D;

	case EATextureType::CUBEMAP:
		return GL_TEXTURE_CUBE_MAP;
	}

	g_log.error("Texture Type not recognized");
	return -1;
	
}

