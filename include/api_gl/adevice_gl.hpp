#pragma once

#include "iadevice.hpp"

namespace AGN
{
	class ADeviceGL : public IADevice
	{
	public:
		ADeviceGL();
		void init() override;
		class AGN::IAMesh* createMesh(struct AGN::MeshData& a_meshData) override;

	};
}
