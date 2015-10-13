#include "asharedh.hpp"
#include "adevice_dx11.hpp"
#include "amesh_dx11.hpp"
#include "atexture_dx11.hpp"
#include "ashader_dx11.hpp"
#include "ashaderpipeline_dx11.hpp"

using namespace glm;

AGN::ADeviceDX11::ADeviceDX11()
{

}

void AGN::ADeviceDX11::init()
{
	
}

AGN::IAMesh* AGN::ADeviceDX11::createMesh(const uint16_t a_aId, AGN::AMeshData* a_meshData)
{
	return nullptr;
}

AGN::IATexture* AGN::ADeviceDX11::createTexture(const uint16_t a_aId, AGN::ATextureData* a_textureData)
{
	return nullptr;
}

AGN::IAShader* AGN::ADeviceDX11::createShader(const uint16_t a_aId, const char* a_shaderSource, AGN::EAShaderType a_type)
{
	return nullptr;
}

AGN::IAShaderPipeline* AGN::ADeviceDX11::createShaderPipeline(const uint16_t a_aId, std::vector<AGN::IAShader*> a_shaders)
{
	return nullptr;
}