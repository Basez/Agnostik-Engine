#pragma once

#include "iashader.hpp"

namespace AGN
{
	class AShaderGL : public IAShader
	{
	public:
		AShaderGL(const uint16_t a_aId, EAShaderType a_type, GLuint a_glId);
		GLuint getGlId() { return m_glId; }
		static GLenum getGlShaderType(EAShaderType a_type);
		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }

	private:
		const uint16_t m_aId;
		EAShaderType m_type;
		GLuint m_glId;
	};
}