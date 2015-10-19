#pragma once

#include "iatexture.hpp"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace AGN
{
	class ATextureDX11 : public IATexture
	{
	public:
		ATextureDX11(const uint16_t a_aId, struct ATextureData* a_textureData, ID3D11Texture2D* a_d3d11TextureHandle, ID3D11ShaderResourceView* a_shaderResourceView);
		~ATextureDX11() override;
		
		std::string getRelativePath() override;
		void setTextureParams(unsigned int a_flags = 0) override;
		uint16_t getId() override { return m_aId; }

		ID3D11Texture2D* getTextureHandle() { return m_textureHandle; }
		ID3D11ShaderResourceView* getShaderResourceView() { return m_shaderResourceView; }

	private:
		const uint16_t m_aId;
		struct ATextureData* m_textureData;
		ID3D11Texture2D* m_textureHandle;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};
}