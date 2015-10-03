#pragma once

#include "iashader.hpp"

namespace AGN
{
	class AShaderGL : public IAShader
	{
	public:
		AShaderGL(const uint16_t a_aId, EAShaderType a_type, GLuint a_glId)
			: m_aId(a_aId)
			, m_type(a_type)
			, m_glId(a_glId) {}

		GLuint getGlId() { return m_glId; }
		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }
		
		static GLenum getGlShaderType(EAShaderType a_type)
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

	private:
		const uint16_t m_aId;
		EAShaderType m_type;
		GLuint m_glId;
	};
}