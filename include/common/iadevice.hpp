#pragma once

namespace AGN
{
	class IADevice
	{
	public:
		virtual void init() = 0;
		virtual class IAMesh* createMesh(struct MeshData& a_meshData) = 0;
		virtual ~IADevice() {}
	};

}
