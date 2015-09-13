#pragma once

namespace AGN
{
	class IAShader
	{
	public:
		virtual ~IAShader() {}
	};

	enum class EAShaderType
	{
		VertexShader,
		PixelShader,
		HullShader,			// TODO: implement later
		DomainShader,		// TODO: implement later
		GeometryShader,		// TODO: implement later
		ComputeShader		// TODO: implement later
	};
}