#pragma once

namespace AGN
{
	enum class EShaderType;

	class IShaderPipeline
	{
	public:
		virtual ~IShaderPipeline() {}
		virtual class IShader* getShader(const EShaderType a_type) = 0;

		virtual uint16_t getAId() = 0;
		virtual void bind() = 0;
		virtual void setConstantBufferData(const EShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) = 0;
		virtual bool hasConstantBuffer(const EShaderType a_shader, const char* a_name) = 0;

	};

	struct ShaderPipelineData
	{
		ShaderPipelineData()
			: aId(0)
			, vertexShader(nullptr)
			, pixelShader(nullptr)
			, hullShader(nullptr)
			, domainShader(nullptr)
			, geometryShader(nullptr)
			, computeShader(nullptr)
		{}

		uint16_t aId;					// required
		class IShader* vertexShader;	// required
		class IShader* pixelShader;	// required
		class IShader* hullShader;
		class IShader* domainShader;
		class IShader* geometryShader;
		class IShader* computeShader;
	};

	
}