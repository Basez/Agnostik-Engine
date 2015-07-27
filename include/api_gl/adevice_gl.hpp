#pragma once

#include "iadevice.hpp"

namespace AGN
{
	class ADeviceGL : public IADevice
	{
	public:
		ADeviceGL();
		void init() override;
	};
}
