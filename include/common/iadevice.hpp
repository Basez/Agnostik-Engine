#pragma once

namespace AGN
{
	class IADevice
	{
	public:
		virtual void init() = 0;
		virtual class IAMesh* createMesh(struct AMeshData* a_meshData) = 0;
		virtual class IATexture* createTexture(struct ATextureData* a_textureData) = 0;
		virtual ~IADevice() {}
	};

}
