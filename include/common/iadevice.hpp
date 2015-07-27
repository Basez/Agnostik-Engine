#pragma once

namespace AGN
{
	class IADevice
	{
	public:
		virtual void init() = 0;

		virtual ~IADevice() {}
	};

}
