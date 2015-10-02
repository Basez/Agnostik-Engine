#pragma once

#include "iadevice.hpp"

namespace AGN
{
	class ADeviceGL : public IADevice
	{
	public:
		ADeviceGL();
		void init() override;
		class IAMesh* createMesh(const uint16_t a_aId, struct AMeshData* a_meshData) override;
		class IATexture* createTexture(const uint16_t a_aId, struct ATextureData* a_textureData) override;
		class IAShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EAShaderType a_type) override;
		class IAShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IAShader*> a_shaders) override;

	};
}
