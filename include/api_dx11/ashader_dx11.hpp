#pragma once

#include "iashader.hpp"

namespace AGN
{
	class AShaderDX11 : public IAShader
	{
	public:
		AShaderDX11() {}

		EAShaderType getType() override { return EAShaderType::VertexShader; } // TODO: hardcoded
		uint16_t getAId() override { return 0; } // TODO:
		
	private:
	};
}