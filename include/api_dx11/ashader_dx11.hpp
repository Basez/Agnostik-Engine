#pragma once

#include "iashader.hpp"

// fwd declare (most ever)
struct ID3D11Device;
struct ID3D11DeviceChild;
struct ID3D10Blob;
struct ID3D11Buffer;
struct D3D11_INPUT_ELEMENT_DESC;
typedef ID3D10Blob ID3DBlob;
struct ID3D11ShaderReflection;
typedef struct _D3D11_SIGNATURE_PARAMETER_DESC D3D11_SIGNATURE_PARAMETER_DESC;
struct D3D11_SAMPLER_DESC;
typedef struct _D3D11_SHADER_BUFFER_DESC D3D11_SHADER_BUFFER_DESC;
typedef struct _D3D11_SHADER_DESC D3D11_SHADER_DESC;

namespace AGN
{
	class AShaderDX11 : public IAShader
	{
	public:
		AShaderDX11(class ADeviceDX11& a_deviceReference, const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle, ID3DBlob* a_shaderBlob);

		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }
		static std::string getLatestProfile(const AGN::EAShaderType a_type, ID3D11Device* a_device);
		ID3D11DeviceChild* getD3D11Shader() { return m_shaderHandle; }
		ID3DBlob* getBlob() { return m_shaderBlob; }
		void setConstantBufferData(const char* a_name, void* a_data, size_t a_dataSize);
		virtual bool hasConstantBuffer(const char* a_name);
		void getInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC*& out_inputLayoutDescs, int& out_count);
		void getSamplerLayoutDesc(D3D11_SAMPLER_DESC*& out_samplerLayoutDecs, int& out_count);

		// SoA stored constant buffers
		ID3D11Buffer** getConstantBufferHandles() { return m_constantBufferHandles; }
		int getNumConstantBuffers() { return m_numConstantBuffers; }

	private:
		class ADeviceDX11& m_deviceReference;
		const uint16_t m_aId;
		EAShaderType m_type;
		ID3D11DeviceChild* m_shaderHandle;
		ID3DBlob* m_shaderBlob;
		ID3D11ShaderReflection* m_shaderReflection;
		D3D11_SHADER_DESC* m_shaderReflectionDesc;
		// SoA stored constant buffers
		int m_numConstantBuffers;
		ID3D11Buffer** m_constantBufferHandles;
		D3D11_SHADER_BUFFER_DESC* m_constantBufferDescriptions;

		void getConstantBufferDesc(D3D11_SHADER_BUFFER_DESC*& out_constantBufferDecs, int& out_count);
		//void getOutputLayoutDesc(D3D11_INPUT_ELEMENT_DESC*& inputLayouts, int& out_count);				// TODO?
		static uint32_t getFormatByParameterSignature(D3D11_SIGNATURE_PARAMETER_DESC& a_signature);
	};
}