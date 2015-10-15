#pragma once

#include "iashader.hpp"

// fwd declare
struct ID3D11Device;
struct ID3D11DeviceChild;
//struct ID3D11PixelShader;
//struct ID3D11VertexShader;

namespace AGN
{
	class AShaderDX11 : public IAShader
	{
	public:
		AShaderDX11(const uint16_t a_aId, EAShaderType a_type, ID3D11DeviceChild* a_shaderHandle);

		EAShaderType getType() override { return m_type; }
		uint16_t getAId() override { return m_aId; }

		static std::string getLatestProfile(const AGN::EAShaderType a_type, ID3D11Device* a_device);
		
	private:
		const uint16_t m_aId;
		EAShaderType m_type;
		ID3D11DeviceChild* m_shaderHandle;

		/*
		union ShaderHandleUnion
		{
			ID3D11PixelShader* pixelShader;
			ID3D11VertexShader* vertexShader;
		} shaderHandleUnion;
		*/

	};
}