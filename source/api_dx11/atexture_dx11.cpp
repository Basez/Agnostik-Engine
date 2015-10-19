#include "asharedh.hpp"
#include "atexture_dx11.hpp"

AGN::ATextureDX11::ATextureDX11(const uint16_t a_aId, ATextureData* a_textureData, ID3D11Texture2D* a_d3d11TextureHandle, ID3D11ShaderResourceView* a_shaderResourceView)
	: m_aId(a_aId)
	, m_textureData(a_textureData)
	, m_textureHandle(a_d3d11TextureHandle)
	, m_shaderResourceView(a_shaderResourceView)
{

}

AGN::ATextureDX11::~ATextureDX11()
{
}

std::string AGN::ATextureDX11::getRelativePath()
{
	return m_textureData->relativePath;
}

void AGN::ATextureDX11::setTextureParams(unsigned int a_flags)
{
	// TODO:
}


