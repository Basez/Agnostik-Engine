#pragma once

#include "iashader.hpp"

namespace AGN
{
	class AShaderGL : public IAShader
	{
	public:
		AShaderGL(const uint16_t a_aId, EAShaderType a_type, uint32_t a_glId);

		uint32_t getGlId() { return m_glId; }
		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }
		static uint32_t getGlShaderType(EAShaderType a_type);
		
	private:
		const uint16_t m_aId;
		EAShaderType m_type;
		uint32_t m_glId;
	};
}