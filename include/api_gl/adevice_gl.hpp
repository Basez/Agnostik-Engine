#pragma once

#include "iadevice.hpp"

namespace AGN
{
	class ADeviceGL : public IADevice
	{
	public:
		ADeviceGL();
		void init() override;
		class IAMesh* createMesh(struct AMeshData* a_meshData) override;
		class IATexture* createTexture(struct ATextureData* a_textureData) override;
	};
}
