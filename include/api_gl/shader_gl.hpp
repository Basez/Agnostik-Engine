#pragma once

#include "i_shader.hpp"

namespace AGN
{
	class ShaderGL : public IShader
	{
	public:
		ShaderGL(const uint16_t a_aId, EShaderType a_type, uint32_t a_glId);

		uint32_t getGlId() { return m_glId; }
		EShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }
		static uint32_t getGlShaderType(EShaderType a_type);
		
	private:
		const uint16_t m_aId;
		EShaderType m_type;
		uint32_t m_glId;
	};
}