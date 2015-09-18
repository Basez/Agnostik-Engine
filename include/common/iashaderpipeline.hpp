#pragma once

#include <memory>

namespace AGN
{
	class IAShaderPipeline
	{
	public:
		virtual ~IAShaderPipeline() {}
		virtual class IAShader* getVertextShader() = 0;
		virtual class IAShader* getPixelShader() = 0;
		virtual class IAShader* getHullShader() = 0;
		virtual class IAShader* getDomainShader() = 0;
		virtual class IAShader* getGeometryShader() = 0;
		virtual class IAShader* getComputeShader() = 0;

		void setProperty(std::string a_propertyName, int a_value) {}; // TODO: set uniforms?
	};

	struct AShaderPipelineData
	{
		AShaderPipelineData()
			: aId(0)
			, vertextShader(nullptr)
			, pixelShader(nullptr)
			, hullShader(nullptr)
			, domainShader(nullptr)
			, geometryShader(nullptr)
			, computeShader(nullptr)
		{}

		uint16_t aId;						// required
		class IAShader* vertextShader;	// required
		class IAShader* pixelShader;	// required
		class IAShader* hullShader;
		class IAShader* domainShader;
		class IAShader* geometryShader;
		class IAShader* computeShader;
	};
}