#pragma once

#include "iashaderpipeline.hpp"

// fwd declare
struct ID3D11InputLayout;
struct ID3D11SamplerState;
struct ID3D11Buffer;
typedef struct _D3D11_SHADER_BUFFER_DESC D3D11_SHADER_BUFFER_DESC;

namespace AGN
{
	static const int MAX_UNIFORM_NAME = 128;

	class AShaderPipelineDX11 : public IAShaderPipeline
	{
	public:
		AShaderPipelineDX11(struct AShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState);

		class IAShader* getShader(const EAShaderType a_type) override;

		uint16_t getAId() { return m_aId; }
		void bind() override;
		void setConstantBufferData(const EAShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasConstantBuffer(const EAShaderType a_shader, const char* a_name) override;
		ID3D11InputLayout* getVertexInputLayout() const { return m_vertexInputLayout; }
		ID3D11SamplerState* getSamplerState() { return m_samplerState; }

	private:
		const uint16_t m_aId;
		class IAShader* m_shaders[6];
		ID3D11InputLayout* m_vertexInputLayout;
		ID3D11SamplerState* m_samplerState;
	};	

	
	struct AConstantBufferDX11
	{
		AConstantBufferDX11() : bufferHandle(nullptr), bufferDesc(nullptr) {}

		ID3D11Buffer* bufferHandle;
		D3D11_SHADER_BUFFER_DESC* bufferDesc;
	};
}