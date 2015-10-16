#pragma once

#include <memory>

namespace AGN
{
	class IAShaderPipeline
	{
	public:
		virtual ~IAShaderPipeline() {}
		virtual class IAShader* getVertexShader() = 0;
		virtual class IAShader* getPixelShader() = 0;
		virtual class IAShader* getHullShader() = 0;
		virtual class IAShader* getDomainShader() = 0;
		virtual class IAShader* getGeometryShader() = 0;
		virtual class IAShader* getComputeShader() = 0;

		virtual uint16_t getAId() = 0;
		virtual void bind() = 0;
		virtual void setConstantBufferData(const char* a_name, void* a_data, size_t a_dataSize) = 0;
		virtual bool hasConstantBuffer(const char* a_name) = 0;

	};

	struct AShaderPipelineData
	{
		AShaderPipelineData()
			: aId(0)
			, vertexShader(nullptr)
			, pixelShader(nullptr)
			, hullShader(nullptr)
			, domainShader(nullptr)
			, geometryShader(nullptr)
			, computeShader(nullptr)
		{}

		uint16_t aId;					// required
		class IAShader* vertexShader;	// required
		class IAShader* pixelShader;	// required
		class IAShader* hullShader;
		class IAShader* domainShader;
		class IAShader* geometryShader;
		class IAShader* computeShader;
	};

	
}