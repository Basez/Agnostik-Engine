#pragma once

namespace AGN
{
	enum class EAShaderType; 

	class IAShader
	{
	public:
		virtual ~IAShader() {}
		virtual EAShaderType getType() = 0;
		virtual uint16_t getAId() = 0;
	};

	enum class EAShaderType
	{
		VertexShader,
		PixelShader,
		HullShader,			// TODO: implement later?
		DomainShader,		// TODO: implement later?
		GeometryShader,		// TODO: implement later?
		ComputeShader		// TODO: implement later?
	};
}