#pragma once

namespace AGN
{
	class IAMaterial
	{
	public:
		virtual ~IAMaterial() {}
		virtual std::string getName() = 0;
	};

	struct AMaterialData
	{
		std::string name;
		std::string vertexShader;
		std::string pixelShader;
	};
}