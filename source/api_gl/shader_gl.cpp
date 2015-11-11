#include "shared.hpp"

// memory leak detection on windows debug builds
#if defined(_WIN32) && defined(AGN_DEBUG) && defined(AGN_ENABLE_MEMORYLEAK_DETECTION)
#include "mmgr.h"
#endif

#include "shader_gl.hpp"

#include <GL/glew.h>

using namespace glm;

AGN::ShaderGL::ShaderGL(const uint16_t a_aId, EShaderType a_type, uint32_t a_glId)
	: m_aId(a_aId)
	, m_type(a_type)
	, m_glId(a_glId)
{

}

AGN::ShaderGL::~ShaderGL()
{
}

uint32_t AGN::ShaderGL::getGlShaderType(EShaderType a_type)
{
	switch (a_type)
	{
	case EShaderType::VertexShader:
		return GL_VERTEX_SHADER;

	case EShaderType::PixelShader:
		return GL_FRAGMENT_SHADER;

	default:
		g_log.error("Unsupported Shader type");
		break;
	}

	return -1;
}
