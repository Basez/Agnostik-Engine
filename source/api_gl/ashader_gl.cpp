#include "asharedh.hpp"
#include "ashader_gl.hpp"

AGN::AShaderGL::AShaderGL(EAShaderType a_type, GLuint a_id)
	: m_type(a_type)
	, m_id(a_id)
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
		g_log.error("Unsupported Shadertype");
		break;			
	}
	
	return -1;
}

