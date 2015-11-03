#pragma once

#include "i_device.hpp"

namespace AGN
{
	class DeviceGL : public IDevice
	{
	public:
		DeviceGL();
		bool init() override;
		class IMesh* createMesh(const uint16_t a_aId, struct MeshData* a_meshData) override;
		class ITexture* createTexture(const uint16_t a_aId, struct TextureData* a_textureData) override;
		class IShader* createShader(const uint16_t a_aId, const char* a_shaderSource, EShaderType a_type) override;
		class IShaderPipeline* createShaderPipeline(const uint16_t a_aId, std::vector<class IShader*> a_shaders) override;

	};
}
