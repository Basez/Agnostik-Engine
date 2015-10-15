#pragma once

#include "iashader.hpp"

// fwd declare (most ever)
struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D10Blob;
struct D3D11_INPUT_ELEMENT_DESC;
typedef ID3D10Blob ID3DBlob;
struct ID3D11ShaderReflection;
typedef struct _D3D11_SIGNATURE_PARAMETER_DESC D3D11_SIGNATURE_PARAMETER_DESC;
struct D3D11_SAMPLER_DESC;
typedef struct _D3D11_SHADER_BUFFER_DESC D3D11_SHADER_BUFFER_DESC;

namespace AGN
{
	class AShaderDX11 : public IAShader
	{
	public:
		AShaderDX11(const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle, ID3DBlob* a_shaderBlob);

		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }

		static std::string getLatestProfile(const AGN::EAShaderType a_type, ID3D11Device* a_device);
		
		void getInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC*& out_inputLayoutDescs, int& out_count);
		//void getOutputLayout(D3D11_INPUT_ELEMENT_DESC* inputLayouts, int& out_count);				// TODO?
		void getSamplerLayoutDesc(D3D11_SAMPLER_DESC*& out_samplerLayoutDecs, int& out_count);
		void getConstantBufferDesc(D3D11_SHADER_BUFFER_DESC*& out_constantBufferDecs, int& out_count);

		ID3DBlob* getBlob() { return m_shaderBlob; }
		

	private:
		const uint16_t m_aId;
		EAShaderType m_type;
		ID3D11DeviceChild* m_shaderHandle;
		ID3DBlob* m_shaderBlob;
		ID3D11ShaderReflection* m_shaderReflection;

		static uint32_t getFormatByParameterSignature(D3D11_SIGNATURE_PARAMETER_DESC& a_signature);
	};
}