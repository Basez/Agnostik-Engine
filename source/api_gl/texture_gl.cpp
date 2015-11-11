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
	setTextureParams(a_textureData->flags);

	// upload to gpu
	if (m_textureData->buffer != nullptr) pushBuffer();
}

AGN::TextureGL::~TextureGL()
{
	g_log.warning("TODO: CLEAN TextureGL::~TextureGL()");

	delete m_textureData;
}

std::string AGN::TextureGL::getRelativePath()
{
	return m_textureData->relativePath;
}

void AGN::TextureGL::setTextureParams(unsigned int a_flags)
{
	glBindTexture(m_glType, m_glId);

	// set parameters
	if (a_flags & (int)ETextureRenderFlags::USE_NEAREST_NEIGBOR)
	{
		glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else if (a_flags & (int)ETextureRenderFlags::USE_MIP_MAPS)
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
	if (a_flags & (int)ETextureRenderFlags::USE_CLAMP)
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

	AGN::RenderAPIGL::getOpenGLErrors();
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
void AGN::TextureGL::pushBuffer()
{
	if (m_textureData->buffer == nullptr)
	{
		g_log.error("Cannot push buffer of this texture, because the buffer is null!");
	}

	glBindTexture(m_glType, m_glId);
	glTexImage2D(m_glType, 0, GL_RGBA, m_textureData->width, m_textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_textureData->buffer);
	glBindTexture(m_glType, 0);

	AGN::RenderAPIGL::getOpenGLErrors();
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

