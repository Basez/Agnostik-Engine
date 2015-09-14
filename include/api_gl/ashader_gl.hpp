#pragma once

#include "iashader.hpp"

namespace AGN
{
	class AShaderGL : public IAShader
	{
	public:
		AShaderGL(EAShaderType a_type, GLuint a_id);
		GLuint getGlID() { return m_id; }
		static GLenum getGlShaderType(EAShaderType a_type);
		
	private:
		EAShaderType m_type;
		GLuint m_id;
	};
}