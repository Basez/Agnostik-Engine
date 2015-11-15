#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "texture_gl.hpp"
#include "render_api_gl.hpp"

// OpenGL includes
#include <GL/glew.h>

AGN::TextureGL::TextureGL(const uint16_t a_id, TextureData* a_textureData, uint32_t a_glId)
	: m_id(a_id)
	, m_textureData(a_textureData)
	, m_glId(a_glId)
{
	m_glType = TextureGL::getGlTypeByTextureType(a_textureData->type);

	// upload to gpu
	if (m_textureData->buffer != nullptr) pushBuffer();
}

AGN::TextureGL::~TextureGL()
{
	delete m_textureData;
}

std::string AGN::TextureGL::getRelativePath()
{
	return m_textureData->relativePath;
}

// pull buffer data from GPU into CPU
void AGN::TextureGL::pullBuffer()
{
	// delete old buffer first
	if (m_textureData->buffer != nullptr)
	{
		delete m_textureData->buffer;
		m_textureData->buffer = nullptr;
	}

	// get buffer from loaded texture
	glBindTexture(m_glType, m_glId);
	const unsigned int size = m_textureData->width * m_textureData->height;
	m_textureData->buffer = new uint32_t[size];
	glGetTexImage(m_glType, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData->buffer);
	glBindTexture(m_glType, 0);

	AGN::RenderAPIGL::getOpenGLErrors();
}

// generates a new texture on the GPU memory with the currently stored pixeldata
// TODO: use glTexSubImage2D instead, should be faster?
// TODO: not sure if we are going to restore this functionality as it is notoriously slow, better to regenerate the whole texture from scratch!
void AGN::TextureGL::pushBuffer()
{
	if (m_textureData->buffer == nullptr)
	{
		g_log.error("Cannot push buffer of this texture, because the buffer is null!");
	}

	//glBindTexture(m_glType, m_glId);
	//glTexImage2D(m_glType, 0, GL_RGBA, m_textureData->width, m_textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData->buffer);
	//glBindTexture(m_glType, 0);

	//AGN::RenderAPIGL::getOpenGLErrors();
}

uint32_t AGN::TextureGL::getGlTypeByTextureType(ETextureType a_type)
{
	switch (a_type)
	{
	case ETextureType::TEXTURE_1D:
		return GL_TEXTURE_1D;

	case ETextureType::TEXTURE_2D:
		return GL_TEXTURE_2D;

	case ETextureType::CUBEMAP:
		return GL_TEXTURE_CUBE_MAP;
	}

	g_log.error("Texture Type not recognized");
	return -1;
	
}

