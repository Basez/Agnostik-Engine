#pragma once

namespace AGN
{
	class IARenderAPI
	{
	public:
		virtual ~IARenderAPI() {}

		virtual void init() = 0;
		virtual void enableVSync(bool a_value) = 0;

		virtual class IAWindow& getWindow() = 0;
		virtual class IADevice& getDevice() = 0;
		virtual class IARenderer& getRenderer() = 0;
	};
}
