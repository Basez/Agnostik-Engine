#pragma once

#include "i_shader_pipeline.hpp"

// fwd declare
struct ID3D11InputLayout;
struct ID3D11SamplerState;
typedef struct _D3D11_SHADER_BUFFER_DESC D3D11_SHADER_BUFFER_DESC;

namespace AGN
{
	class ShaderPipelineDX11 : public IShaderPipeline
	{
	public:
		ShaderPipelineDX11(struct ShaderPipelineData* a_shaderPipelineData, ID3D11InputLayout* a_vertexInputLayout, ID3D11SamplerState* a_samplerState);
		~ShaderPipelineDX11() override;

		class IShader* getShader(const EShaderType a_type) override;

		uint16_t getAId() { return m_aId; }
		void bind() override;
		void setConstantBufferData(const EShaderType a_shader, const char* a_name, void* a_data, size_t a_dataSize) override;
		virtual bool hasConstantBuffer(const EShaderType a_shader, const char* a_name) override;
		ID3D11InputLayout* getVertexInputLayout() const { return m_vertexInputLayout; }
		ID3D11SamplerState* getSamplerState() { return m_samplerState; }
		struct ConstantBufferDX11* getConstantBufferByName(const EShaderType a_shader, const char* a_name);

	private:
		const uint16_t m_aId;
		class IShader* m_shaders[6];
		ID3D11InputLayout* m_vertexInputLayout;
		ID3D11SamplerState* m_samplerState;
	};	
}