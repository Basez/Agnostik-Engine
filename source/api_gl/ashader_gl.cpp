#include "asharedh.hpp"
#include "ashader_gl.hpp"

AGN::AShaderGL::AShaderGL(const uint16_t a_aId, EAShaderType a_type, GLuint a_glId)
	: m_aId(a_aId)
	, m_type(a_type)
	, m_glId(a_glId)
{

}

GLenum AGN::AShaderGL::getGlShaderType(EAShaderType a_type)
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

