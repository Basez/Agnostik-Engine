#pragma once

namespace AGN
{
	enum class EShaderType; 

	class IShader
	{
	public:
		virtual ~IShader() {}
		virtual EShaderType getType() = 0;
		virtual uint16_t getAId() = 0;
	};

	enum class EShaderType
	{
		VertexShader,
		PixelShader,
		HullShader,			// TODO: implement later?
		DomainShader,		// TODO: implement later?
		GeometryShader,		// TODO: implement later?
		ComputeShader		// TODO: implement later?
	};
}