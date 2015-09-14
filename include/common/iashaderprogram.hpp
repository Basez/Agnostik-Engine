#pragma once

namespace AGN
{
	class IAShaderProgram
	{
	public:
		virtual ~IAShaderProgram() {}
	};

	struct AShaderProgramData
	{
		std::string vertexShader;
		std::string pixelShader;
	};
}