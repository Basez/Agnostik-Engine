#include "asharedh.hpp"
#include "atexture_dx11.hpp"

AGN::ATextureDX11::ATextureDX11(const uint16_t a_id, ATextureData* a_textureData, ID3D11Texture2D* a_d3d11TextureHandle)
	: m_id(a_id)
	, m_textureData(a_textureData)
	, m_textureHandle(a_d3d11TextureHandle)
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


