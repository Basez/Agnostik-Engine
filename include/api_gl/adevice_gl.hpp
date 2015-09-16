#pragma once

#include "iadevice.hpp"

namespace AGN
{
	class ADeviceGL : public IADevice
	{
	public:
		ADeviceGL();
		void init() override;
		class IAMesh* createMesh(const uint16_t a_id, struct AMeshData* a_meshData) override;
		class IATexture* createTexture(const uint16_t a_id, struct ATextureData* a_textureData) override;
		class IAShader* createShader(const char* a_shaderSource, EAShaderType a_type) override;
		class IAMaterial* createMaterial(const uint16_t a_id, std::string a_name, std::vector<class IAShader*> a_shaders) override;

	};
}
