#pragma once

#include "i_shader.hpp"

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
	class ShaderDX11 : public IShader
	{
	public:
		ShaderDX11(class DeviceDX11& a_deviceReference, const uint16_t a_aId, EShaderType a_type, ID3D11DeviceChild* a_shaderHandle, ID3DBlob* a_shaderBlob);
		~ShaderDX11() override;

		EShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }
		static std::string getLatestProfile(const AGN::EShaderType a_type, ID3D11Device* a_device);
		ID3D11DeviceChild* getD3D11Shader() { return m_shaderHandle; }
		ID3DBlob* getBlob() { return m_shaderBlob; }
		void setConstantBufferData(const char* a_name, void* a_data, size_t a_dataSize);
		virtual bool hasConstantBuffer(const char* a_name);
		void getInputLayoutDesc(D3D11_INPUT_ELEMENT_DESC*& out_inputLayoutDescs, int& out_count);
		void getSamplerLayoutDesc(D3D11_SAMPLER_DESC*& out_samplerLayoutDecs, int& out_count);

		uint32_t getNumConstantBuffers() { return static_cast<uint32_t>(m_constantBuffers.size()); }
		struct ConstantBufferDX11* getConstantBufferByName(const char* a_name);
		ID3D11Buffer** getConstantBufferHandles() { return &m_bufferHandles[0]; } // makes binding easier

		static const int MAX_UNIFORM_NAME = 128;

	private:
		void getConstantBufferDesc(D3D11_SHADER_BUFFER_DESC*& out_constantBufferDecs, int& out_count);
		static uint32_t getFormatByParameterSignature(D3D11_SIGNATURE_PARAMETER_DESC& a_signature);

		class DeviceDX11& m_deviceReference;
		const uint16_t m_aId;
		EShaderType m_type;
		ID3D11DeviceChild* m_shaderHandle;
		ID3DBlob* m_shaderBlob;
		ID3D11ShaderReflection* m_shaderReflection;
		D3D11_SHADER_DESC* m_shaderReflectionDesc;
		std::vector<ID3D11Buffer*> m_bufferHandles; // makes binding easier
		std::vector<struct ConstantBufferDX11*> m_constantBuffers;
	};

	struct ConstantBufferPropertyDX11
	{
		char name[ShaderDX11::MAX_UNIFORM_NAME];
		uint32_t size;
		uint32_t offset;
	};

	struct ConstantBufferDX11
	{
		ConstantBufferPropertyDX11* getPropertyByName(const char* a_name);

		char name[ShaderDX11::MAX_UNIFORM_NAME];
		uint32_t size;
		int32_t bindpoint;
		ID3D11Buffer* d3dhandle;
		uint8_t* buffer;
		
		std::vector<struct ConstantBufferPropertyDX11*> propertyList;
	};
}