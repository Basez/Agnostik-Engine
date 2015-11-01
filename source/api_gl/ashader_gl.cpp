#include "asharedh.hpp"
#include "ashader_gl.hpp"

#include <GL/glew.h>

using namespace glm;

AGN::AShaderGL::AShaderGL(const uint16_t a_aId, EAShaderType a_type, uint32_t a_glId)
	: m_aId(a_aId)
	, m_type(a_type)
	, m_glId(a_glId)
{

}

uint32_t AGN::AShaderGL::getGlShaderType(EAShaderType a_type)
{
	switch (a_type)
	{
	case EAShaderType::VertexShader:
		return GL_VERTEX_SHADER;

	case EAShaderType::PixelShader:
		return GL_FRAGMENT_SHADER;

	default:
		g_log.error("Unsupported Shader type");
		break;
	}

	return -1;
}
