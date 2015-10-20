#pragma once

#include <memory>

namespace AGN
{
	enum class EAShaderType;

	class IAShaderPipeline
	{
	public:
		virtual ~IAShaderPipeline() {}
		virtual class IAShader* getShader(const EAShaderType a_type) = 0;

		virtual uint16_t getAId() = 0;
		virtual void bind() = 0;
		virtual void setConstantBufferData(const EAShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) = 0;
		virtual bool hasConstantBuffer(const EAShaderType a_shader, const char* a_name) = 0;

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