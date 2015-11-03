#include "shared.hpp"
#include "texture_dx11.hpp"

AGN::TextureDX11::TextureDX11(const uint16_t a_aId, TextureData* a_textureData, ID3D11Texture2D* a_d3d11TextureHandle, ID3D11ShaderResourceView* a_shaderResourceView)
	: m_aId(a_aId)
	, m_textureData(a_textureData)
	, m_textureHandle(a_d3d11TextureHandle)
	, m_shaderResourceView(a_shaderResourceView)
{

}

AGN::TextureDX11::~TextureDX11()
{
}

std::string AGN::TextureDX11::getRelativePath()
{
	return m_textureData->relativePath;
}

void AGN::TextureDX11::setTextureParams(unsigned int a_flags)
{
	// TODO:
}


