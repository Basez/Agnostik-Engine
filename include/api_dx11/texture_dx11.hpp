#pragma once

#include "i_texture.hpp"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace AGN
{
	class TextureDX11 : public ITexture
	{
	public:
		TextureDX11(const uint16_t a_aId, struct TextureData* a_textureData, ID3D11Texture2D* a_d3d11TextureHandle, ID3D11ShaderResourceView* a_shaderResourceView);
		~TextureDX11() override;
		
		std::string getRelativePath() override;
		void setTextureParams(unsigned int a_flags = 0) override;
		uint16_t getId() override { return m_aId; }

		ID3D11Texture2D* getTextureHandle() { return m_textureHandle; }
		ID3D11ShaderResourceView* getShaderResourceView() { return m_shaderResourceView; }

	private:
		const uint16_t m_aId;
		struct TextureData* m_textureData;
		ID3D11Texture2D* m_textureHandle;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};
}